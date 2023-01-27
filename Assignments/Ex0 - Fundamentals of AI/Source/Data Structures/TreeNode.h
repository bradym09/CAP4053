#include <iostream>
#include <queue>
#include <vector>

// TreeNode class should go in the "ufl_cap4053::fundamentals" namespace!
namespace ufl_cap4053 { namespace fundamentals {
	using namespace std;

	template <class T> class TreeNode {
	private:
		T value;
		vector<TreeNode<T>*> children;

	public:
		TreeNode<T>() {
			value = nullptr;
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
			return children[index];
		}

		void addChild(TreeNode<T>* child) {
			children.push_back(child);
		}

		void removeChild(size_t index) {
			children.erase(children.begin() + index);
		}

		void breadthFirstTraverse(void (*dataFunction)(const T)) const {
			
		}

		void preOrderTraverse(void (*dataFunction)(const T)) const {

		}

		void postOrderTraverse(void (*dataFunction)(const T)) const {

		}
	};
}}  // namespace ufl_cap4053::fundamentals
