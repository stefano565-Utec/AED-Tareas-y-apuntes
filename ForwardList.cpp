#include <iostream>
#include <stdexcept>  // Para lanzar excepciones

using namespace std;

template<typename T>
struct Node{
    T data;
    Node* next;
    
    // Constructor para inicializar el nodo
    Node(T val) : data(val), next(nullptr){}
};

template <typename T>
class ForwardList{
private:
    Node<T>* head;  // Puntero al primer nodo de la lista
    int list_size;  // Tamaño de la lista

    // Función para fusionar dos listas ordenadas
    Node<T>* merge(Node<T>* left, Node<T>* right) {
        if (left == nullptr) return right;
        if (right == nullptr) return left;

        Node<T>* result = nullptr;

        if (left->data <= right->data) {
            result = left;
            result->next = merge(left->next, right);
        } else {
            result = right;
            result->next = merge(left, right->next);
        }
        return result;
    }

    // Función recursiva para realizar Merge Sort en la lista enlazada
    Node<T>* mergeSort(Node<T>* node) {
        // Caso base: si la lista es vacía o tiene un solo nodo
        if (node == nullptr || node->next == nullptr) {
            return node;
        }

        // Encontrar el punto medio de la lista
        Node<T>* middle = getMiddle(node);
        Node<T>* nextOfMiddle = middle->next;

        // Separar la lista en dos mitades
        middle->next = nullptr;

        // Ordenar cada mitad recursivamente
        Node<T>* left = mergeSort(node);
        Node<T>* right = mergeSort(nextOfMiddle);

        // Fusionar las dos mitades ordenadas
        return merge(left, right);
    }

    // Función para encontrar el punto medio de la lista
    Node<T>* getMiddle(Node<T>* node) {
        if (node == nullptr) {
            return nullptr;
        }

        Node<T>* slow = node;
        Node<T>* fast = node->next;

        // Avanzar `fast` dos pasos y `slow` un paso hasta que `fast` llegue al final
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        return slow;
    }
public:
    // Constructor: Inicializa la lista como vacía
    ForwardList() : head(nullptr), list_size(0){}

    // Destructor: Libera la memoria ocupada por los nodos
    ~ForwardList(){
        clear();
    }

    // Retorna el elemento al comienzo de la lista
    T front() const{
        if(empty()){
            throw out_of_range("Lista vacía");
        }
        return head->data;
    }

    // Retorna el elemento al final de la lista
    T back() const{
        if(empty()){
            throw out_of_range("Lista vacía");
        }
        Node<T>* current = head;
        while(current->next != nullptr){
            current = current->next;
        }
        return current->data;
    }

    // Inserta un elemento al frente de la lista
    void push_front(T val){
        Node<T>* newNode = new Node<T>(val);
        newNode->next = head;
        head = newNode;
        ++list_size;
    }

    // Inserta un elemento al final de la lista
    void push_back(T val){
        Node<T>* newNode = new Node<T>(val);

        if(head == nullptr){
            head = newNode;
        }else{
            Node<T>* current = head;
            while (current->next != nullptr){
                current = current->next;
            }
            current->next = newNode;
        }
        ++list_size;
    }

    // Elimina y retorna el primer elemento de la lista
    T pop_front(){
        if(empty()){
            throw out_of_range("Lista vacía");
        }
        Node<T>* temp = head;
        T value = head->data;
        head = head->next;
        delete temp;
        --list_size;
        return value;
    }

    // Elimina y retorna el último elemento de la lista
    T pop_back(){
        if(empty()){
            throw out_of_range("Lista vacía");
        }

        if(head->next == nullptr){  // Si solo hay un nodo
            T value = head->data;
            delete head;
            head = nullptr;
            --list_size;
            return value;
        }

        Node<T>* current = head;
        while(current->next->next != nullptr){  // Recorrer hasta el penúltimo nodo
            current = current->next;
        }
        T value = current->next->data;
        delete current->next;
        current->next = nullptr;
        --list_size;
        return value;
    }

    // Retorna el elemento en la posición indicada
    T operator[](int index) const{
        if(index < 0 || index >= list_size){
            throw out_of_range("Índice fuera de rango");
        }
        Node<T>* current = head;
        for(int i = 0; i < index; ++i){
            if(current == nullptr){
                throw out_of_range("Índice fuera de rango");
            }
        current = current->next;
        }

        return current->data;
    }

    // Retorna si la lista está vacía o no
    bool empty() const{
        return head == nullptr;
    }

    // Retorna el tamaño de la lista
    int size() const{
        return list_size;
    }

    // Elimina todos los elementos de la lista
    void clear(){
        while (!empty()){
            pop_front();
        }
    }

    // Invierte la lista enlazada
    void reverse(){
        if(empty() || head->next == nullptr){
            return;
        }
        Node<T>* prev = nullptr;
        Node<T>* current = head;
        Node<T>* next = nullptr;

        while(current != nullptr){
            next = current->next;  // Guardar el siguiente nodo
            current->next = prev;  // Invertir el puntero
            prev = current;        // Mover prev y current hacia adelante
            current = next;
        }
        head = prev;  // Actualizar el head al último nodo
    }

    // Ordena la lista usando el algoritmo de burbuja
    /*
    void sort(){
        if(empty() || head->next == nullptr){
            return;
        }
        
        bool swapped;
        do{
            swapped = false;
            Node<T>* current = head;
            
            while(current->next != nullptr){
                if(current->data > current->next->data){
                    // Intercambiar los valores
                    swap(current->data, current->next->data);
                    swapped = true;
                }
                current = current->next;
            }
        }while(swapped);
    }
    */

    // Método para ordenar la lista usando Merge Sort
    void sort(){
        head = mergeSort(head);
    }

    // Método para imprimir los elementos de la lista
    void print() const{
        Node<T>* current = head;
        while(current != nullptr){
            cout << current->data << " -> ";
            current = current->next;
        }
        cout << "nullptr" << endl;
    }
};

int main(){
    ForwardList<int> list;

    list.push_back(10);
    list.push_back(5);
    list.push_back(15);
    list.push_front(20);

    cout << "Lista original:" << endl;
    list.print();  // 20 -> 10 -> 5 -> 15 -> nullptr

    cout << "Primer elemento: " << list.front() << endl;  // 20
    cout << "Ultimo elemento: " << list.back() << endl;   // 15

    cout << "Elemento en la posicion 2: " << list[2] << endl;  // 5

    list.pop_front();
    list.pop_back();
    cout << "Lista despues de eliminar el primer y ultimo elemento:" << endl;
    list.print();  // 10 -> 5 -> nullptr

    list.push_back(3);
    list.push_back(8);
    list.push_back(1);

    cout << "Lista antes de ordenar:" << endl;
    list.print();  // 10 -> 5 -> 3 -> 8 -> 1 -> nullptr

    list.sort();
    cout << "Lista despues de ordenar:" << endl;
    list.print();  // 1 -> 3 -> 5 -> 8 -> 10 -> nullptr

    list.reverse();
    cout << "Lista despues de invertir:" << endl;
    list.print();  // 10 -> 8 -> 5 -> 3 -> 1 -> nullptr

    cout << "Tamanio de la lista: " << list.size() << endl;  // 5

    list.clear();
    cout << "Lista despues de limpiar todos los elementos:" << endl;
    list.print();  // nullptr
    cout << "La lista esta vacia: " << (list.empty() ? "Si" : "No") << endl;  // Sí

    return 0;
}




