#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product {
    int code;
    char name[50];
    float price;
    struct Product* next;  
};

void enqueue(struct Product** front, struct Product** rear, struct Product newProduct) {
    struct Product* newProductNode = (struct Product*)malloc(sizeof(struct Product));
    if (newProductNode == NULL) {
        perror("Erro ao alocar memoria");
        exit(EXIT_FAILURE);
    }

    *newProductNode = newProduct;
    newProductNode->next = NULL;

    if (*rear == NULL) {
        *front = *rear = newProductNode;
    } else {
        (*rear)->next = newProductNode;
        *rear = newProductNode;
    }
}

void createProduct(struct Product** front, struct Product** rear) {
    struct Product newProduct;

    puts("Digite o codigo do produto: ");
    scanf("%d", &newProduct.code);
    puts("Digite o nome do produto: ");
    scanf("%s", newProduct.name);
    puts("Digite o preco do produto: ");
    scanf("%f", &newProduct.price);

    enqueue(front, rear, newProduct);

    puts("Produto cadastrado com sucesso");
}

void removeProduct(struct Product** front, struct Product** rear) {
    if (*front == NULL) {
        puts("A fila esta vazia, nenhum produto para remover");
        return;
    }

    struct Product* removedProduct = *front;
    *front = removedProduct->next;
    free(removedProduct);

    puts("Produto removido com sucesso");
}

void showProducts(struct Product* front) {
    if (front == NULL) {
        puts("A fila esta vazia, nenhum produto para mostrar");
        return;
    }

    struct Product* current = front;
    while (current != NULL) {
        printf("%d | %s | %.2f\n", current->code, current->name, current->price);
        current = current->next;
    }
}

void saveProductsFile(struct Product* front) {
    FILE *archiver = fopen("db/products.bin", "wb");
    if (archiver == NULL) {
        perror("Erro ao abrir o arquivo de produtos");
        exit(EXIT_FAILURE);
    }

    struct Product* current = front;
    while (current != NULL) {
        fwrite(current, sizeof(struct Product), 1, archiver);
        current = current->next;
    }

    fclose(archiver);
    puts("Fila salva no arquivo com sucesso");
}

void loadProductsFile(struct Product** front, struct Product** rear) {
    FILE *archiver = fopen("db/products.bin", "rb");
    if (archiver == NULL) {
        perror("Erro ao abrir o arquivo de produtos");
        exit(EXIT_FAILURE);
    }

    struct Product product;
    while (fread(&product, sizeof(struct Product), 1, archiver) == 1) {
        enqueue(front, rear, product);
    }

    fclose(archiver);
    puts("Fila carregada do arquivo com sucesso");
}

void menu() {
    puts("-----------------------------------");
    puts("[1] - Cadastrar Produto");
    puts("[2] - Remover produto");
    puts("[3] - Mostrar Produtos");
    puts("[4] - Salvar fila do sistema para o arquivo");
    puts("[5] - Carregar fila do arquivo para o sistema");
    puts("[0] - Sair");
    puts("-----------------------------------");
    puts("Opcao: ");
    puts("-----------------------------------");
}

void process_menu(int option, struct Product** front, struct Product** rear) {
    switch(option) {
        case 1:
            createProduct(front, rear);
            break;
        case 2:
            removeProduct(front, rear);
            break;
        case 3:
            showProducts(*front);
            break;
        case 4:
            saveProductsFile(*front);
            break;
        case 5:
            loadProductsFile(front, rear);
            break;
        case 0:
            puts("Saindo do sistema...");
            exit(EXIT_SUCCESS);
            break;
        default:
            puts("Opcao invalida");
            break;
    }
}

int main() {
    struct Product* front = NULL;
    struct Product* rear = NULL;

    int option;
    do {
        menu();
        scanf("%d", &option);
        process_menu(option, &front, &rear);
    } while (option != 0);

    return 0;
}
