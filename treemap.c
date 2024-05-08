#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * nuevoArbol = (TreeMap *)malloc(sizeof(TreeMap)); //Se crea el árbol y se reserva memoria para este
    nuevoArbol->root = NULL; //Se asigna NULL a la raíz del árbol
    nuevoArbol->current = NULL; //Se asigna NULL al nodo actual
    nuevoArbol->lower_than = lower_than; //Se inicializa la función para comparar claves
    return nuevoArbol; //Retorna el nuevo árbol

    //new->lower_than = lower_than;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * nuevoNodo = createTreeNode(key, value); //Creamos un nuevo nodo con la clave y el valor entregados
    if (tree->root == NULL){ //Si la raíz es NULL, el nuevo nodo será la raíz
        tree->root = nuevoNodo;
    }
    else{
        TreeNode * aux = tree->root; //Creamos un nodo auxiliar que apunte a la raíz
        while (aux != NULL){ //Mientras el nodo auxiliar no sea NULL
            if (tree->lower_than(key, aux->pair->key) == 1){ //Si la clave del nodo auxiliar es menor que la clave del nuevo nodo
                if (aux->left == NULL){ //Si el nodo izquierdo del nodo auxiliar es NULL
                    nuevoNodo->parent = aux;
                    aux->left = nuevoNodo;
                    break;
                }
                else{
                    aux = aux->left;
                }
            }
            else{
                if (aux->right == NULL){
                    nuevoNodo->parent = aux;
                    aux->right = nuevoNodo;
                    break;
                }
                else{
                    aux = aux->right;
                }
            }
        }
    }
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL){
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *current = tree->root;

    // Recorrido del árbol hasta llegar a un nodo nulo o encontrar la clave buscada.
    while (current != NULL) {
        // Comparación de la clave buscada con la clave del nodo actual.
        // Comparación de la clave buscada con la clave del nodo actual usando la función lower_than_int.
        if (tree->lower_than(key, current->pair->key)) {
            // Si la clave buscada es menor, ir al subárbol izquierdo.
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) {
            // Si la clave buscada es mayor, ir al subárbol derecho.
            current = current->right;
        } else {
            // Si las claves son iguales, se ha encontrado el nodo buscado.
            return current->pair;
        }

    }

    // Si se llega aquí, la clave no está presente en el árbol.
    return NULL;
}



Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
