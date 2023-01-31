#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// TreeNode class should go in the "ufl_cap4053::fundamentals" namespace!
namespace ufl_cap4053 { namespace fundamentals {
	template <typename T> class TreeNode {
	private:
		T value;
		vector<TreeNode<T>*> children;

	public:
		TreeNode<T>() {
			value = NULL;
		}

		TreeNode<T>(T element) {
			value = element;
		}

		const T& getData() const {
			return value;
		}

		size_t getChildCount() const {
			return children.size();
		}

		TreeNode<T>* getChild(size_t index) {
			return children[index];
		}

		TreeNode<T>* getChild(size_t index) const {
			return this->children[index];
		}

		void addChild(TreeNode<T>* child) {
			children.push_back(child);
		}

		void removeChild(size_t index) {
			children.erase(children.begin() + index);
		}

		void breadthFirstTraverse(void (*dataFunction)(const T)) const {
			queue<const TreeNode<T>*> queue;
			const TreeNode<T>* root = this;
			queue.emplace(root);

			while (!queue.empty()) {
				const TreeNode<T>* curr = queue.front();
				queue.pop();
				dataFunction(curr->getData());

				for (int i = 0; i < curr->getChildCount(); i++) {
					TreeNode<T>* child = curr->getChild(i);
					queue.push(child);
				}
			}
		}

		void preOrderTraverse(void (*dataFunction)(const T)) const {
			dataFunction(this->getData());

			for (int i = 0; i < children.size(); i++) {
				children[i]->preOrderTraverse(dataFunction);
			}
		}

		void postOrderTraverse(void (*dataFunction)(const T)) const {
			for (int i = 0; i < children.size(); i++) {
				children[i]->postOrderTraverse(dataFunction);
			}

			dataFunction(this->getData());
		}
	};
}}  // namespace ufl_cap4053::fundamentals
