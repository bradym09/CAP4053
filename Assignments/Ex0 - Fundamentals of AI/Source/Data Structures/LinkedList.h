#include <iostream>

using namespace std;

// LinkedList class should go in the "ufl_cap4053::fundamentals" namespace!
namespace ufl_cap4053 {
    namespace fundamentals {
        template <typename T> class LinkedList {
        private:
            struct Node {
                T element;
                Node* next;
                Node* prev;
                Node(T element, Node* next, Node* previous) {
                    this->element = element;
                    prev = previous;
                    this->next = next;
                }
            };

            Node* start;
            Node* stop;
            Node* curr;

        public:
            LinkedList<T>() {
                curr = nullptr;
                start = nullptr;
                stop = nullptr;
            }

            ~LinkedList<T>() {
                Node* current = start;
                while (current != nullptr) {
                    Node* temp = current->next;
                    delete current;
                    current = temp;
                }
            }

            class Iterator {
            private:
                Node* current;

                Node* endState;

            public:
                friend class LinkedList;

                T operator*() const {
                    //cout << "Element: " << current->element << " retrieved!\n";
                    return current->element;
                }

                Iterator& operator++() {
                    if (current->next != nullptr) {
                        current = current->next;
                        return *this;
                    }
                    current = nullptr;
                    return *this;
                }

                bool operator==(Iterator const& rhs) {
                    if (rhs.current->element == this->current->element)
                        return true;
                    return false;
                }

                bool operator!=(Iterator const& rhs) {
                    cout << this->current << endl;
                    if (this->current == rhs.current || this->current->element != rhs.current->element)
                        return true;
                    cout << "false";
                    return false;
                }
            };

            Iterator begin() const {
                Iterator iter = Iterator();
                iter.current = start;
                //cout << "BegiN: " << iter.current->element << endl;
                return iter;
            }

            Iterator end() const {
                Iterator iter = Iterator();
                iter.current = nullptr;
                return iter;
            }

            bool isEmpty() const {
                if (start == nullptr && stop == nullptr)
                    return true;
                return false;
            }

            T getFront() const {
                Node front = *start;
                return front.element;
            }

            T getBack() const {
                Node back = *stop;
                return back.element;
            }

            void enqueue(T element) {
                //Add to front if first element
                if (start == nullptr) {
                    start = new Node(element, nullptr, nullptr);
                    stop = start;
                }
                else {
                    Node* temp = stop->prev;
                    stop = new Node(element, nullptr, temp);
                }
            }

            void dequeue() {
                Node* temp = start->next;
                temp->prev = nullptr;
                delete start;
                start = temp;
            }

            void pop() {
                Node* temp = stop->prev;
                temp->next = nullptr;
                delete stop;
                stop = temp;
            }

            void clear() {
                Node* curr = start;
                while (curr != nullptr) {
                    Node* temp = curr->next;
                    curr->next = nullptr;
                    temp->prev = nullptr;
                    delete curr;
                    curr = temp;
                }
            }

            bool contains(T element) const {
                Node* curr = start;
                while (curr != nullptr) {
                    if (curr->element == element)
                        return true;
                    curr = curr->next;
                }

                return false;
            }

            void remove(T element) {
                Node* curr = start;
                while (curr != nullptr) {
                    if (curr->element == element) {
                        Node* temp = curr->next;
                        Node* prev = curr->prev;

                        prev->next = temp;
                        temp->prev = prev;

                        curr->next = nullptr;
                        curr->prev = nullptr;
                        delete curr;

                        return;
                    }
                }
            }

            
        };
        
    }
}  // namespace ufl_cap4053::fundamentals