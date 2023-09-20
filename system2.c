#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>

#define CODEN_LEN 20
#define DESC_LEN 50
#define GROUP_LEN 30
#define LOCAL_LEN 30
#define UNIT_LEN 30
#define DATABASE "dataBase.txt"

typedef struct Product
{
    char codeNumber[CODEN_LEN];
    char description[DESC_LEN];
    char group[GROUP_LEN];
    float minimumStock;
    float quantity;
    char localization[LOCAL_LEN];
    char unitMeasurement[UNIT_LEN];
} Product;


int printMenu();
void printHeader(char screenName[30]);
void printFooter(char screenName[50]);
void redirectPage(int option);
int addProduct();
int editProduct();
int deleteProduct();
void registerEntry();
void registerExit();
int listProducts();
int listProductsToBuy();
int registerMovimentation(char movType[10]);
int searchProduct();
void displayMemoryError();

int main()
{
    setlocale(LC_ALL, "Portuguese_Brazil");
    system("color 8F");

    int option = printMenu();
    redirectPage(option);

    return 0;
}

int printMenu()
{
    int option;

    printHeader("Menu");
    printf("\t\t1 - Cadastrar novo produto\n");
    printf("\t\t2 - Editar produto\n");
    printf("\t\t3 - Excluir produto\n");
    printf("\t\t4 - Listar produtos\n");
    printf("\t\t5 - Listar produtos a comprar\n");
    printf("\t\t6 - Registrar entrada de produto\n");
    printf("\t\t7 - Registrar sa�da de produto\n");
    printf("\t\t8 - Procurar por um produto\n");
    printf("\t\t9 - Sair\n");
    printFooter("Menu");
    printf("\t\tEscolha uma op��o: ");
    scanf("%d", &option);

    return option;
}

void redirectPage(int option)
{
    switch (option)
    {
    case 1:
        addProduct();
        break;
    case 2:
        editProduct();
        break;
    case 3:
        deleteProduct();
        break;
    case 4:
        listProducts();
        break;
    case 5:
        listProductsToBuy();
        break;
    case 6:
        registerEntry();
        break;
    case 7:
        registerExit();
        break;
    case 8:
        searchProduct();
        break;
    case 9:
        printf("\n\t\t");
        break;
    default:
        printf("\n\t\tC�digo invp�lido!");
        system("cls");
        main();
    }
}

void printHeader(char screenName[50])
{
    system("cls");
    printf("\n\t------------------------------ %s ------------------------------\n\n", screenName);
}

void printFooter(char screenName[50])
{
    printf("\n\n\t------------------------------ %s ------------------------------\n\n", screenName);
}

int addProduct()
{
    char registerAnother;

    // Vari�veis utilizadas para realizar a aloca��o din�mica
    char *codeNumber, charactere;
    int i = 0;

    codeNumber = malloc(sizeof(char));
    if (!codeNumber) displayMemoryError();

    Product productInfo;
    FILE *filePointerAppend;
    FILE *filePointerRead;

    do
    {
        printHeader("Cadastro de produto");


        filePointerAppend = fopen(DATABASE, "ab");
        filePointerRead = fopen(DATABASE, "rb");
        if (!filePointerAppend || !filePointerRead)
        {
            printf("Erro ao abrir o banco de dados!");
            return main();
        }

        fflush(stdin);

        printf("\t\tDigite o c�digo: ");


        // Aloca��o din�mica de mem�ria //
        do
        {
            charactere = getc(stdin);
            codeNumber[i] = charactere;
            i++;
            codeNumber = (char *)realloc(codeNumber, sizeof(char) * i);
            if (!codeNumber) displayMemoryError();
        } while (charactere != '\n');

        codeNumber[i] = '\0';
        // Aloca��o din�mica de mem�ria //

        fflush(stdin);

        while (fread(&productInfo, sizeof(Product), 1, filePointerRead))
        {
            if (strcmp(productInfo.codeNumber, codeNumber) == 0)
            {
                printf("\n\n\t _________________________________________________________________________________\n");
                printf("\n\t\tO c�digo digitado j� existe!! Verifique e tente novamente.");
                printf("\n\n\t _________________________________________________________________________________\n\n\t\t");
                system("pause");
                return main();
            }
        }
        fclose(filePointerRead);
        strcpy(productInfo.codeNumber, codeNumber);

        printf("\n\t\tDigite a descri��o: ");
        gets(productInfo.description);
        fflush(stdin);
        printf("\n\t\tDigite o grupo: ");
        gets(productInfo.group);
        fflush(stdin);
        printf("\n\t\tDigite a localiza��o: ");
        gets(productInfo.localization);
        fflush(stdin);
        printf("\n\t\tDigite o estoque m�nimo: ");
        scanf("%f", &productInfo.minimumStock);
        fflush(stdin);
        printf("\n\t\tDigite a unidade de medida: ");
        scanf("%s", productInfo.unitMeasurement);
        fflush(stdin);
        productInfo.quantity = 0;

        fwrite(&productInfo, sizeof(Product), 1, filePointerAppend);
        fclose(filePointerAppend);

        printf("\n\n\t_________________________________________________________________________________\n");
        printf("\n\t\tProduto cadastrado com sucesso!");
        printf("\n\n\t_________________________________________________________________________________\n\n");
        printFooter("Cadastro de produto");

        printf("\t\tQuer cadastrar mais um produto? (S/N): ");
        scanf("%c", &registerAnother);
        fflush(stdin);
    } while (registerAnother == 's' || registerAnother == 'S');


    free(codeNumber);
    return main();
}

void displayMemoryError()
{
    printf("\n\t\tErro ao alocar mem�ria!");
    system("pause");
    main();
}

int deleteProduct()
{
    char *codeNumber;
    codeNumber = malloc(sizeof(char *) * CODEN_LEN);

    if (!codeNumber)
        displayMemoryError();

    char confirmation;
    char deleteAnother;
    int found;
    Product productInfo;

    do
    {
        found = 0;
        printHeader("Exclus�o de Produto");

        printf("\t\tDigite o c�digo do item que deseja excluir: ");
        scanf("%s", codeNumber);
        fflush(stdin);
        printf("\n\t\tTem certeza disso? (S/N): ");
        scanf("%c", &confirmation);
        fflush(stdin);

        if (confirmation != 's' && confirmation != 'S')
        {
            return main();
        }

        FILE *filePointer, *filePointerTemp;

        filePointer = fopen(DATABASE, "rb");
        filePointerTemp = fopen("temp.txt", "wb");

        while (fread(&productInfo, sizeof(Product), 1, filePointer))
        {
            if (strcmp(codeNumber, productInfo.codeNumber) == 0)
            {
                found = 1;
            }
            else
            {
                fwrite(&productInfo, sizeof(Product), 1, filePointerTemp);
            }
        }

        fclose(filePointer);
        fclose(filePointerTemp);

        if (found == 1)
        {
            remove(DATABASE);
            rename("temp.txt", DATABASE);
            printf("\n\n\t_________________________________________________________________________________\n");

            printf("\n\t\tProduto exclu�do com sucesso!");
            printf("\n\n\t_________________________________________________________________________________\n\n");
        }
        if (found == 0)
        {
            remove("temp.txt");
            printf("\n\n\t_________________________________________________________________________________\n");
            printf("\n\t\tErro: Produto n�o encontrado!");
            printf("\n\n\t_________________________________________________________________________________\n\n");
        }

        printFooter("Exclus�o de Produto");
        printf("\n\t\tDeseja excluir outro produto? (S/N): ");
        scanf("%c", &deleteAnother);
        fflush(stdin);

    } while (deleteAnother == 's' || deleteAnother == 'S');

    free(codeNumber);
    return main();
}

int listProducts()
{
    printHeader("Lista de produtos");

    int item = 1;
    FILE *filePointer;
    Product productInfo;

    filePointer = fopen(DATABASE, "rb");
    if (!filePointer)
    {
        printf("\n\t\tErro ao abrir o banco de dados!\n\n\t\t");
        system("pause");
        return main();
    }

    while (fread(&productInfo, sizeof(Product), 1, filePointer))
    {
        printf("\n\t\t Item %d\n", item);
        printf("\t_______________________________________________________________________________\n\n");
        printf("\t\tC�digo: %s\n", productInfo.codeNumber);
        printf("\t\tDescri��oo: %s\n", productInfo.description);
        printf("\t\tGrupo: %s\n", productInfo.group);
        printf("\t\tLocaliza��o: %s\n", productInfo.localization);
        printf("\t\tEstoque m�nimo: %0.2f\n", productInfo.minimumStock);
        printf("\t\tUnidade de medida: %s\n", productInfo.unitMeasurement);
        printf("\t\tEstoque f�sico: %0.2f\n", productInfo.quantity);
        printf("\n\t_______________________________________________________________________________\n");
        item++;
    }
    fclose(filePointer);
    printFooter("Lista de produtos");
    printf("\t\t");
    system("pause");
    return main();
}

int listProductsToBuy()
{
    printHeader("Lista de produtos a comprar");

    FILE *filePointer;
    Product productInfo;

    filePointer = fopen(DATABASE, "rb");
    if (!filePointer)
    {
        printf("Erro ao abrir o banco de dados!");
        return 1;
    }

    while (fread(&productInfo, sizeof(Product), 1, filePointer))
    {

        if (productInfo.quantity <= productInfo.minimumStock)
        {
            printf("\n\t__________________________________________________________________________________________\n\n");
            printf("\t\tC�digo: %s\n", productInfo.codeNumber);
            printf("\t\tDescri��o: %s\n", productInfo.description);
            printf("\t\tUnidade de medida: %s\n", productInfo.unitMeasurement);
            printf("\t\tEstoque m�nimo: %0.2f\n", productInfo.minimumStock);
            printf("\t\tQuantidade em estoque: %0.2f\n", productInfo.quantity);
            printf("\t\tStatus: Comprar\n");
            printf("\n\t__________________________________________________________________________________________\n");
        }
    }

    fclose(filePointer);
    printFooter("Lista de produtos a comprar");

    printf("\t\t");
    system("pause");
    return main();
}

void registerEntry()
{
    registerMovimentation("entry");
}
void registerExit()
{
    registerMovimentation("exit");
}

int registerMovimentation(char movType[10])
{
    char *codeNumber;
    codeNumber = malloc(sizeof(char *) * CODEN_LEN);
    if (!codeNumber)
        displayMemoryError();

    float quantity;
    char anotherEntry;
    FILE *filePointer;
    Product productInfo;

    do
    {
        if ((strcmp(movType, "entry") == 0))
        {
            printHeader("Registrar entrada");
        }
        else if (strcmp(movType, "exit") == 0)
        {
            printHeader("Registrar sa�da");
        }

        int id = 0;
        int found = 0;
        printf("\n\t\tDigite o c�digo do produto: ");
        scanf("%s", codeNumber);
        fflush(stdin);

        if ((strcmp(movType, "entry") == 0))
        {
            printf("\n\n\t\tDigite a quantidade a ser adicionada: ");
        }
        else if ((strcmp(movType, "exit") == 0))
        {
            printf("\n\n\t\tDigite a quantidade a ser subtra�da: ");
        }
        scanf("%f", &quantity);
        fflush(stdin);

        filePointer = fopen(DATABASE, "rb+");
        if (!filePointer)
        {
            printf("\t\tErro ao abrir o banco de dados\n\t\t!");
            system("pause");
            return main();
        }

        while (fread(&productInfo, sizeof(Product), 1, filePointer))
        {
            if (strcmp(productInfo.codeNumber, codeNumber) == 0)
            {
                if (strcmp(movType, "entry") == 0)
                {
                    productInfo.quantity += quantity;
                }
                else if (strcmp(movType, "exit") == 0)
                {
                    productInfo.quantity -= quantity;
                }

                fseek(filePointer, id * sizeof(Product), SEEK_SET);
                fwrite(&productInfo, sizeof(Product), 1, filePointer);

                printf("\n\n\t_________________________________________________________________________________\n");
                printf("\n\t\tMovimenta��o realizada com sucesso!");
                printf("\n\n\t_________________________________________________________________________________\n");

                found = 1;
                break;
            }
            id++;
        }

        if (found == 0)
        {
            printf("\n\n\t_______________________________________________________________________________\n");
            printf("\n\t\tProduto n�o encontrado!");
            printf("\n\n\t_______________________________________________________________________________\n\n\t\t");
            system("pause");
            return main();
        }

        fclose(filePointer);

        if ((strcmp(movType, "entry") == 0))
        {
            printf("\n\n\t\tDeseja cadastrar mais uma entrada? (S/N): ");
        }
        else if (strcmp(movType, "exit") == 0)
        {
            printf("\n\n\t\tDeseja cadastrar mais uma sa�da? (S/N): ");
        }
        scanf("%c", &anotherEntry);

    } while (anotherEntry == 's' || anotherEntry == 'S');

    free(codeNumber);
    return main();
}

int editProduct()
{
    int id;
    int found;
    char editAnother;
    char *codeNumber;
    codeNumber = malloc(sizeof(char *) * CODEN_LEN);
    if (!codeNumber)
        displayMemoryError();

    Product productInfo;
    FILE *filePointer;

    do
    {
        printHeader("Edi��o de produtos");
        id = 0;
        found = 0;

        filePointer = fopen(DATABASE, "rb+");
        if (!filePointer)
        {
            printf("\t\tErro ao abrir o banco de dados! ");
            return main();
        }

        printf("\t\tDigite o c�digo que deseja alterar: ");
        scanf("%s", codeNumber);
        fflush(stdin);

        while (fread(&productInfo, sizeof(Product), 1, filePointer))
        {
            if (strcmp(codeNumber, productInfo.codeNumber) == 0)
            {
                printf("\n\n\t_______________________________________________________________________________\n\n");

                printf("\t\tC�digo: %s\n\n", productInfo.codeNumber);
                printf("\t\tDescri��o: %s\n", productInfo.description);
                printf("\t\tGrupo: %s\n", productInfo.group);
                printf("\t\tUnidade de medida: %s\n", productInfo.unitMeasurement);
                printf("\t\tEstoque m�nimo: %2.f\n", productInfo.minimumStock);
                printf("\t\tLocaliza��o: %s\n", productInfo.localization);

                printf("\n\n\t_______________________________________________________________________________\n\n");

                printf("\n\t\tDigite a nova descri��o: ");
                gets(productInfo.description);
                fflush(stdin);
                printf("\n\t\tDigite o novo grupo: ");
                gets(productInfo.group);
                fflush(stdin);
                printf("\n\t\tDigite a nova unidade de medida: ");
                scanf("%s", productInfo.unitMeasurement);
                fflush(stdin);
                printf("\n\t\tDigite o novo estoque m�nimo: ");
                scanf("%f", &productInfo.minimumStock);
                fflush(stdin);
                printf("\n\t\tDigite a nova localiza��o: ");
                gets(productInfo.localization);
                fflush(stdin);

                found = 1;
                fseek(filePointer, id * sizeof(Product), SEEK_SET);
                fwrite(&productInfo, sizeof(Product), 1, filePointer);

                printf("\n\n\t_______________________________________________________________________________\n");
                printf("\n\t\tProduto %s alterado com sucesso!", codeNumber);
                printf("\n\n\t_______________________________________________________________________________\n\n");
                break;
            }
            id++;
        }

        if (found == 0)
        {
            printf("\n\n\t_______________________________________________________________________________\n");
            printf("\n\t\tProduto n�o encontrado! Verifique e tente novamente.");
            printf("\n\t_______________________________________________________________________________\n\n\t\t");
            system("pause");
            return main();
        }

        fclose(filePointer);

        printFooter("Edi��o de produtos");

        printf("\t\tDeseja editar outro produto? (S/N): ");
        scanf("%c", &editAnother);
        fflush(stdin);
    } while (editAnother == 's' || editAnother == 'S');

    free(codeNumber);
    return main();
}

int searchProduct()
{
    Product productInfo;
    FILE *filePointer;
    char *codeNumber;
    codeNumber = malloc(sizeof(char *) * CODEN_LEN);

    if (!codeNumber)
        displayMemoryError();

    char option;
    int found;

    do
    {
        printHeader("Procurar por produto");

        found = 0;
        filePointer = fopen(DATABASE, "rb");
        if (!filePointer)
        {
            printf("\t\tErro ao abrir o banco de dados! ");
            return main();
        }

        printf("\n\t\tDigite o c�digo que deseja procurar: ");
        scanf("%s", codeNumber);

        while (fread(&productInfo, sizeof(Product), 1, filePointer))
        {
            if (strcmp(productInfo.codeNumber, codeNumber) == 0)
            {
                printf("\n\n\t__________________________________________________________________________________\n");
                printf("\n\t\tDescri��o: %s", productInfo.description);
                printf("\n\t\tGrupo %s", productInfo.group);
                printf("\n\t\tEstoque m�nimo: %0.2f", productInfo.minimumStock);
                printf("\n\t\tQuantidade em estoque: %0.2f", productInfo.quantity);
                printf("\n\t\tUnidade de medida: %s", productInfo.unitMeasurement);
                printf("\n\n\t__________________________________________________________________________________\n\n");
                found = 1;
            }
        }
        printFooter("Procurar por produto");

        if (found == 0)
        {
            printf("\n\t__________________________________________________________________________________\n");
            printf("\n\t\tProduto n�o encontrado!!");
            printf("\n\n\t__________________________________________________________________________________\n\n\t\t");
            system("pause");
            return main();
        }
        fclose(filePointer);
        fflush(stdin);
        printf("\n\t\tQuer procurar por mais um produto? (S/N): ");
        scanf("%c", &option);

    } while (option == 's' || option == 'S');

    free(codeNumber);
    return main();
}