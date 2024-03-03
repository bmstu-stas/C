#include <stdio.h> //библиотека для ввода-вывода
#include <stdlib.h> // бибилиотека для работы с памятью
#include <string.h> // библиотека для работы с строками
#include "ekzlib.h" // отображение меню

struct PHONE {  // Создание структуры для книги
    int point;      // номер пункта
    char *fname;    // имя
    char *lname;    // фамилия
    int number;     // телефон
    char *comment;  // комментарий
};

int main() {
    struct PHONE phone;
    phone.fname = (char *)malloc(100 * sizeof(char));
    if (phone.fname == NULL) {
        printf("Error memory");
        return 1;
    }
    phone.lname = (char *)malloc(100 * sizeof(char));
    if (phone.lname == NULL) {
        printf("Error memory");
        return 1;
    }
    phone.comment = (char *)malloc(100 * sizeof(char));
    if (phone.comment == NULL) {
        printf("Error memory");
        return 1;
    }

    static int currentPoint; //хранение номера индекса

    // Открытие файла phonebook.txt для добавления данных в конец файла
    FILE *phonebookFile = fopen("phonebook.txt", "a+");
    if (phonebookFile == NULL) {
        printf("Error opened phonebook file");
        return 1;
    }
    // Открытие файла counter.txt для чтения и записи
    FILE *counterFile = fopen("counter.txt", "r+");
    if (counterFile == NULL) {
        // Если файл не существует, создать
        counterFile = fopen("counter.txt", "w+");
        if (counterFile == NULL) {
            printf("Error opened counter file");
            fclose(phonebookFile);
            return 1;
        }
    }
    // Чтение текущего значения индекса из файла counter.txt
    fscanf(counterFile, "%d", &currentPoint);

    int input; // переменная, в которую передается выбор пользователя
    menu();    // отображение меню для выбора
    printf("Input: ");
    scanf("%d", &input); // выбор пользователя

    switch (input) {
        // добавление записи в телефонную книгу
        case 1:
            phone.point = ++currentPoint;

            printf("Enter first name: ");
            scanf("%s", phone.fname);

            printf("Enter last name: ");
            scanf("%s", phone.lname);

            printf("Enter phone number: ");
            scanf("%d", &phone.number);

            printf("Enter comment: ");
            scanf("%s", phone.comment);

            // Запись данных в файл
            fprintf(phonebookFile, "%d %s %s %d %s\n", phone.point, phone.fname, phone.lname, phone.number, phone.comment);

            // Обновление файла счетчика
            fclose(counterFile);
            counterFile = fopen("counter.txt", "r+");
            if (counterFile == NULL) {
                printf("Error opened counter file");
                fclose(phonebookFile);
                return 1;
            }

            fprintf(counterFile, "%d", currentPoint);
            fseek(counterFile, 0, SEEK_SET);
            fprintf(counterFile, "%d", currentPoint);

            break;

        case 2: // поиск определенной записи
            fseek(phonebookFile,0,SEEK_SET);

            char searchName[100]; // сохранение слова поиска
            printf("Enter name or last name to search: ");
            scanf("%s", searchName);

            printf("Search results:\n");

            while (fscanf(phonebookFile, "%d %s %s %d %s", &phone.point, phone.fname, phone.lname, &phone.number, phone.comment) == 5) { // если введены все 5 параметров
                if (strcmp(phone.fname, searchName) == 0 || strcmp(phone.lname, searchName) == 0) {
                    printf("#: %d, Name: %s, Last Name: %s, Phone: %d, Comment: %s\n", phone.point, phone.fname, phone.lname, phone.number, phone.comment);
                }
            }
            break;

        case 3: // показ всей телефонной книги
            fseek(phonebookFile,0,SEEK_SET);


            printf("All records:\n");

            while (fscanf(phonebookFile, "%d %s %s %d %s", &phone.point, phone.fname, phone.lname, &phone.number, phone.comment) == 5) {
                printf("# %d, Name: %s, Last Name: %s, Phone: %d, Comment: %s\n", phone.point, phone.fname, phone.lname, phone.number, phone.comment);
            }

            break;

        case 4: // удаление записи
            fseek(phonebookFile, 0, SEEK_SET);

            int recordToDelete;
            printf("Enter record number to delete: ");
            scanf("%d", &recordToDelete);

            //создание временного файла для хранения результата исключения записи
            FILE *tempFile = fopen("temp.txt", "w");
            if (tempFile == NULL) {
                printf("Error opening temporary file");
                fclose(phonebookFile);
                fclose(counterFile);
                return 1;
            }

            int newCounter = 0; // новый счетчик для обновленных записей

            while (fscanf(phonebookFile, "%d %s %s %d %s", &phone.point, phone.fname, phone.lname, &phone.number, phone.comment) == 5) {
                if (phone.point != recordToDelete) {
                    // перезаписываем запись во временный файл с новым счетчиком
                    fprintf(tempFile, "%d %s %s %d %s\n", ++newCounter, phone.fname, phone.lname, phone.number, phone.comment);
                }
            }

            fclose(phonebookFile);
            fclose(tempFile);

            // Удаляем старый файл и переименовываем временный
            remove("phonebook.txt");
            rename("temp.txt", "phonebook.txt");

            // Обновляем счетчик в файле

            fseek(counterFile, 0, SEEK_SET);
            fprintf(counterFile, "%d", newCounter);

            fclose(counterFile);
            break;

        case 5: //выход из программы при выборе 5
            return 0;



        default:
            printf("Incorrect input");
    }

    fclose(phonebookFile);
    fclose(counterFile);

    free(phone.fname);
    free(phone.lname);
    free(phone.comment);

    return 0;
}
