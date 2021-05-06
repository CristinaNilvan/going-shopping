#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBER_OF_PRODUCTS 1000

typedef struct
{
    char Name[30];
    float Price;
} ProductFeatures;

FILE * OpenFile(const char * FileName)
{
    FILE * File = fopen(FileName, "r");

    return File;
}

float GetAmountOfMoney(FILE * File)
{
    float AmountOfMoney;

    fscanf(File, "%f", &AmountOfMoney);

    return AmountOfMoney;
}

ProductFeatures * GetProductsFromFile(FILE * File, int * NumberOfProducts)
{
    ProductFeatures * Products = (ProductFeatures *)malloc(MAX_NUMBER_OF_PRODUCTS * sizeof(ProductFeatures));

    char Line[50];
    int Index = 0;

    while (fgets(Line, 50, File))
    {
        int Position = 1;
        char * Word = strtok(Line, ",");

        while (Word != NULL)
        {
            if (Position == 1)
                strcpy(Products[Index].Name, Word);

            if (Position == 2) {
                Products[Index].Price = (float)strtod(Word, NULL);

                Index += 1;
            }

            Position += 1;
            Word = strtok(NULL, ",");
        }
    }

    *NumberOfProducts = Index;

    return Products;
}

void DisplayProducts(ProductFeatures * Products, int NumberOfProducts)
{
    for (int i = 0; i < NumberOfProducts; ++i)
        printf("%s %0.2f\n", Products[i].Name, Products[i].Price);
}

void SwapProducts(ProductFeatures * FirstProduct, ProductFeatures * SecondProduct)
{
    ProductFeatures Auxiliary;

    Auxiliary = *FirstProduct;
    *FirstProduct = *SecondProduct;
    *SecondProduct = Auxiliary;
}

void SortProducts(ProductFeatures * Products, int NumberOfProducts)
{
    for (int i = 0; i < NumberOfProducts; ++i)
        for (int j = i + 1; j < NumberOfProducts; ++j)
            if (Products[i].Price > Products[j].Price)
                SwapProducts(&Products[i], &Products[j]);
}

int CountPurchasedProducts(ProductFeatures * Products, int NumberOfProducts, float AmountOfMoney)
{
    int PurchasedProducts = 0;

    for (int i = 0; i < NumberOfProducts; ++i)
        if (AmountOfMoney > 0 && (AmountOfMoney - Products[i].Price > 0))
        {
            AmountOfMoney -= Products[i].Price;
            PurchasedProducts += 1;
        }

    return PurchasedProducts;
}

void StartShopping(FILE * File)
{
    float AmountOfMoney = GetAmountOfMoney(File);

    int NumberOfProducts;
    ProductFeatures * Products = GetProductsFromFile(File, &NumberOfProducts);

    printf("At the store we have these products : \n");
    DisplayProducts(Products, NumberOfProducts);

    SortProducts(Products, NumberOfProducts);

    int PurchasedProducts = CountPurchasedProducts(Products, NumberOfProducts, AmountOfMoney);
    printf("\nNumber of purchased products : %d\n", PurchasedProducts);

    printf("The purchased products are : \n");
    DisplayProducts(Products, PurchasedProducts);

    free(Products);
}

int main()
{
    FILE * File = OpenFile("Products.txt");

    if (!File)
    {
        printf("File not found.");
        exit(5);
    }

    StartShopping(File);

    fclose(File);

    return 0;
}
