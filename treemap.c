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


void insertTreeMap(TreeMap* tree, void* key, void* value) {
    // Realizamos una búsqueda para encontrar el lugar donde debería ubicarse el nuevo nodo
    TreeNode* parent = NULL;
    TreeNode* current = tree->root;

    while (current != NULL) {
        // Comparación de la clave del nodo actual con la clave del nuevo dato
        if (tree->lower_than(key, current->pair->key)) {
            // Si la clave del nuevo dato es menor, seguimos la búsqueda hacia la izquierda
            parent = current;
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) {
            // Si la clave del nuevo dato es mayor, seguimos la búsqueda hacia la derecha
            parent = current;
            current = current->right;
        } else {
            // Si la clave ya existe en el árbol, retornamos sin hacer nada
            return;
        }
    }

    // Creamos el nuevo nodo
    TreeNode* new_node = createTreeNode(key, value);
    if (new_node == NULL) {
        // Manejo de error: no se pudo crear el nuevo nodo
        return;
    }

    // Enlazamos el nuevo nodo en el lugar correcto
    if (parent == NULL) {
        // Si el árbol estaba vacío, el nuevo nodo será la raíz
        tree->root = new_node;
    } else if (tree->lower_than(key, parent->pair->key)) {
        // Si la clave del nuevo nodo es menor que la clave del nodo padre, lo enlazamos como hijo izquierdo
        parent->left = new_node;
    } else {
        // Si la clave del nuevo nodo es mayor que la clave del nodo padre, lo enlazamos como hijo derecho
        parent->right = new_node;
    }

    // Establecemos el puntero al padre del nuevo nodo
    new_node->parent = parent;

    // Actualizamos current al nuevo nodo insertado
    tree->current = new_node;
}



TreeNode * minimum(TreeNode * x){
    while (x->left != NULL){ //Mientras el nodo tenga un hijo izquierdo, avanzamos
        x = x->left;
    }
    return x; //Retornamos el nodo más a la izquierda 
}


void removeNode(TreeMap* tree, TreeNode* node) {
    if (node == NULL) {
        return;
    }

    // Caso 1: Nodo sin hijos
    if (node->left == NULL && node->right == NULL) {
        // Verificamos si el nodo es la raíz del árbol
        if (node == tree->root) {
            tree->root = NULL;
        } else {
            // Anulamos el puntero del padre que apuntaba al nodo
            if (node->parent->left == node) {
                node->parent->left = NULL;
            } else {
                node->parent->right = NULL;
            }
        }
        free(node->pair);
        free(node);
    }
    // Caso 2: Nodo con un hijo
    else if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;

        // Verificamos si el nodo es la raíz del árbol
        if (node == tree->root) {
            tree->root = child;
            child->parent = NULL;
        } else {
            // El padre del nodo pasa a ser el padre de su hijo
            if (node->parent->left == node) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
            child->parent = node->parent;
        }
        free(node->pair);
        free(node);
    }
    // Caso 3: Nodo con dos hijos
    else {
        // Descendemos al hijo derecho y obtenemos el menor nodo del subárbol (minimum)
        TreeNode* successor = minimum(node->right);

        // Reemplazamos los datos (clave-valor) de node con los del nodo "mínimo"
        node->pair->key = successor->pair->key;
        node->pair->value = successor->pair->value;

        // Eliminamos el nodo "mínimo"
        removeNode(tree, successor);
    }
}


void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return; //Si el árbol está vacío o no existe, retornamos NULL

    if (searchTreeMap(tree, key) == NULL) return; //Si la clave no existe, retornamos NULL
    TreeNode* node = tree->current;
    removeNode(tree, node); //Eliminamos el nodo

}




Pair* searchTreeMap(TreeMap* tree, void* key) {
    TreeNode* current = tree->root;

    // Búsqueda en el árbol
    while (current != NULL) {
        // Comparación de la clave buscada con la clave del nodo actual
        if (tree->lower_than(key, current->pair->key)) {
            // Si la clave buscada es menor, se sigue la búsqueda hacia la izquierda
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) {
            // Si la clave buscada es mayor, se sigue la búsqueda hacia la derecha
            current = current->right;
        } else {
            // Si las claves son iguales, se ha encontrado el par buscado
            // Se actualiza current al nodo encontrado y se retorna el par asociado a ese nodo
            tree->current = current;
            return current->pair;
        }
    }

    // Si no se encuentra la clave, se actualiza current a NULL
    tree->current = NULL;

    // Se retorna NULL
    return NULL;
}

Pair* upperBound(TreeMap* tree, void* key) {
    TreeNode* current = tree->root;
    TreeNode* ub_node = NULL;  // Nodo con la menor clave mayor o igual a key

    // Búsqueda normal en el árbol
    while (current != NULL) {
        // Comparación de la clave buscada con la clave del nodo actual
        if (tree->lower_than(key, current->pair->key)) {
            // Si la clave buscada es menor, se guarda el nodo actual y se sigue la búsqueda hacia la izquierda
            ub_node = current;
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) {
            // Si la clave buscada es mayor, se sigue la búsqueda hacia la derecha
            current = current->right;
        } else {
            // Si las claves son iguales, se ha encontrado el par buscado
            return current->pair;
        }
    }

    // Si no se encontró la clave exacta, retorna el par del nodo con la menor clave mayor o igual a key
    if (ub_node != NULL) {
        return ub_node->pair;
    }

    // Si no hay ningún nodo mayor o igual a key, retorna NULL
    return NULL;
}


Pair* firstTreeMap(TreeMap* tree) {
    TreeNode* current = tree->root;

    // Recorremos hacia el nodo más a la izquierda del árbol
    while (current != NULL && current->left != NULL) {
        current = current->left;
    }

    // Si current es NULL, el árbol está vacío
    if (current == NULL) {
        return NULL;
    }

    // Se retorna el primer par encontrado
    return current->pair;
}


Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
