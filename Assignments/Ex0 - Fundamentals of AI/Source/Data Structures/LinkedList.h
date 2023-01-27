

// LinkedList class should go in the "ufl_cap4053::fundamentals" namespace!
namespace ufl_cap4053 {
    namespace fundamentals {
        template <class T> class LinkedList {
        private:
            struct Node {
                T element;
                Node* next, prev;
                Node(T element, Node* next, Node* previous) {
                    this->element = element;
                    prev = previous;
                    this->next = next;
                }
            };

            Node* start, end, curr;

        public:
            LinkedList<T>() {
                curr = nullptr;
                start = nullptr;
                end = nullptr;
            }

            Iterator begin() const {
                return start;
            }

            Iterator end() const {
                return end;
            }

            bool isEmpty() const {
                if (start == nullptr && end == nullptr)
                    return true;
                return false;
            }

            T getFront() const {
                Node front = *start;
                return front.element;
            }

            T getBack() const {
                Node back = *end;
                return back.element;
            }

            void enqueue(T element) {
                //Add to front if first element
                if (start == nullptr)
                    start = Node(element, nullptr, nullptr);
                else {
                    Node* temp = end;
                    end = Node(element, nullptr, temp);
                }
            }

            void dequeue() {
                Node* temp = start->next;
                temp->prev = nullptr;
                delete start;
                start = temp;
            }

            void pop() {
                Node* temp = end->prev;
                temp->next = nullptr;
                delete end;
                end = temp;
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

            class Iterator {
            private:
                Node* current;

            public:
                T operator*() const {
                    return current->element;
                }

                Iterator& operator++() {
                    current = current->next;
                    if (current == nullptr)
                        return LinkedList<T>::end();

                }

                bool operator==(Iterator const& rhs) {
                    if (rhs.current->element == this->current->element)
                        return true;
                    return false;
                }

                bool operator!=(Iterator const& rhs) {
                    if (this->current->element != rhs.current->element)
                        return true;
                    return false;
                }
            };
        };
        
    }
}  // namespace ufl_cap4053::fundamentals