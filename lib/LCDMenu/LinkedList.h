#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdio.h>

template<typename T>
class LinkedList{

    public:
        template<typename NodeT>
        struct Node{
            NodeT data;
            Node<NodeT> *next;
        };
        LinkedList();
        ~LinkedList();

        /**
         * \brief Add a new node to the list
         * \param node The node to be added
         * \param index Position node should be added
         *        in. If index is greater than 
         *        the current size, it is added to
         *        the end. If not provided, node 
         *        will be added to the end.
        **/
        void add(T data, int index = -1);

        /**
         * \brief Return size of list
         * \return int Number of nodes in list
         */
        int size();
        
        /**
         * \brief Return node at given index and remove it.
         *        If index == -1, pop last node and return it.
         * \param index The index of the node to be popped. 
         *              Will default to _size if not provided
         *              or if index == -1
         */
        T pop(int index = -1);

        /** 
         * \brief Return data of node at given index
         * \param index Index of desired node
         * \return T template typename T of data at desired node
         */
        T get(int index);

        /**
         * \brief Attempt to remove a node at a given
         *        index. Returns true on success,
         *        false if index is out of range.
         * \param index Index of node to be deleted.
         */
        bool remove(int index);

        /**
         * \brief Destroy all nodes and set root and last
         *        to NULL and set _size to 0.
         */
        void clear();

        /**
         * \brief Future implementation.
         */
        void sort();

    private:
        /**
         * \brief Pointer to root node. Null
         *        if _size is 0.
         */
        Node<T>* root;
        
        /**
         * \brief Pointer to last node. Null
         *        if _size is 0.
         */
        Node<T>* last;
        
        /**
         * \brief Return a pointer to node
         *        at given index.
         * \param index Index of desired node.
         */
        Node<T>* getNode(int index) const;

        /**
         * \brief Current number of nodes
         */
        int _size;
};

template<typename T>
LinkedList<T>::LinkedList(){
    root = NULL;
    last = NULL;
    _size = 0;
}

template<typename T>
LinkedList<T>::~LinkedList(){
    // clear();
}

template<typename T>
void LinkedList<T>::add(T data, int index){
    Node<T>* newNode = new Node<T>;
    newNode->data = data;

    if(!root){

        // Root has not been created yet
        newNode->next = NULL;
        root = newNode;
        last = newNode;
        _size++;

    }
    else{

        if(index == -1){

            // Add node to end of list
            newNode->next = getNode(index);
            last->next = newNode;
            last = newNode;
            _size++;

        } 
        
        else if(index == 0){
            
            // Add node at beginning of list
            newNode->next = root;
            root = newNode;
            _size++;
        
        }

        else{

            // Add node at specified index
            // printf("\n\nSize: %d\nIndex: %d\nData: %d\n\tBefore: %d\n\tAfter: %d\n\n", _size, index, data, getNode(index-1)->data, getNode(index)->data);
            newNode->next = getNode(index);
            getNode(index-1)->next = newNode;
            _size++;

        }
    }
}

template<typename T>
int LinkedList<T>::size(){
    return _size;
}

template<typename T>
T LinkedList<T>::pop(int index){
    T tmp;
    if(index < 0){

        // Pop the last index
        tmp = last->data;
        remove(_size);

    }else{

        // Pop node at index
        tmp = getNode(index)->data;
        remove(index);

    }

    return tmp;
}

template<typename T>
T LinkedList<T>::get(int index){
    return getNode(index)->data;
}

template<typename T>
LinkedList<T>::Node<T>* LinkedList<T>::getNode(int index) const {
    // Take care of a few low-hanging fruit
    if(index == 0) return root;
    if(index == _size) return last;
    if(index < 0 || index > _size) return NULL;

    int currentIndex = 0;
    Node<T>* currentNode = root;

    while(currentIndex < index && currentNode){
        currentNode = currentNode->next;
        currentIndex++;
    }
    return currentNode;
}

template<typename T>
bool LinkedList<T>::remove(int index){
    if(index > _size || index < 0) return false;

    if(index == _size){

        // Remove last node
        getNode(_size - 1)->next = NULL;
        delete(last);
        _size--;

    }else if(index == 0){
        
        // Remove root node
        delete(root);
        _size--;

    }else{

        // Remove node at index
        Node<T>* tmp = getNode(index-1);
        tmp->next = getNode(index+1);
        delete(getNode(index));
        _size--;

    }

    return true;
}

template<typename T>
void LinkedList<T>::clear(){
    int idx = 0;
    while(_size > 0){
        remove(idx);
    }
    root = NULL;
    last = NULL;
}

template<typename T>
void LinkedList<T>::sort(){
    
}


#endif