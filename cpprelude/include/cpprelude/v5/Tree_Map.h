#pragma once

#include "cpprelude/defines.h"
#include "cpprelude/defaults.h"
#include "cpprelude/v5/Ranges.h"
#include "cpprelude/v5/Memory_Context.h"
#include "cpprelude/v5/OS.h"
#include <utility>

namespace cpprelude
{
	namespace internal
	{
		template<typename T>
		struct Red_Black_Tree_Node
		{
			using Data_Type = T;

			enum Color_Type: bool { RED, BLACK };
			Red_Black_Tree_Node<T> *left, *right, *parent;
			T data;
			Color_Type color;

			Red_Black_Tree_Node()
				:left(nullptr),
				 right(nullptr),
				 parent(nullptr),
				 color(RED)
			{}

			Red_Black_Tree_Node(const T& node_data, Color_Type node_color = Color_Type::RED)
				:left(nullptr),
				 right(nullptr),
				 parent(nullptr),
				 data(node_data),
				 color(node_color)
			{}

			Red_Black_Tree_Node(T&& node_data, Color_Type node_color = Color_Type::RED)
				:left(nullptr),
				 right(nullptr),
				 parent(nullptr),
				 data(std::move(node_data)),
				 color(node_color)
			{}
		};
	}

	template<typename TKey, typename TValue>
	struct Pair_Node
	{
		TKey key;
		TValue value;

		Pair_Node() {}

		Pair_Node(const TKey& k)
			:key(k)
		{}

		Pair_Node(TKey&& k)
			:key(std::move(k))
		{}

		Pair_Node(const TKey& k, const TValue& v)
			:key(k), value(v)
		{}

		Pair_Node(TKey&& k, const TValue& v)
			:key(std::move(k)), value(v)
		{}

		Pair_Node(const TKey& k, TValue&& v)
			:key(k), value(std::move(v))
		{}

		Pair_Node(TKey&& k, TValue&& v)
			:key(std::move(k)), value(std::move(v))
		{}

		bool
		operator<(const Pair_Node& other) const
		{
			return key < other.key;
		}

		bool
		operator>(const Pair_Node& other) const
		{
			return other.key < key;
		}

		bool
		operator==(const Pair_Node& other) const
		{
			return key == other.key;
		}

		bool
		operator!=(const Pair_Node& other) const
		{
			return key != other.key;
		}
	};

	/**
	 * @brief      A Generic Red Black Tree
	 *
	 * @tparam     T                   Values type in the tree
	 * @tparam     TCompare            Compare function used by the tree
	 */
	template<typename T,
			 typename TCompare = default_less_than<T>,
			 typename TNode = internal::Red_Black_Tree_Node<T>,
			 typename TColor = typename TNode::Color_Type,
			 typename TIterator = Red_Black_Tree_Iterator<const TNode>,
			 typename TConstIterator = TIterator,
			 typename TConstFreeDataType = T>
	struct Red_Black_Tree
	{
		/**
		 * Data Type of the tree
		 */
		using Data_Type = T;
		using Node_Type = TNode;
		using Color_Type = TColor;
		/**
		 * Range Type of the tree
		 */
		using Range_Type = Tree_Range<Node_Type>;
		/**
		 * Const Range Type of the tree
		 */
		using Const_Range_Type = Tree_Range<const Node_Type>;

		/**
		 * Iterator type of the tree
		 */
		using iterator = TIterator;

		/**
		 * Const iterator type of the tree
		 */
		using const_iterator = TConstIterator;

		Node_Type *_root;
		usize _count;
		Memory_Context mem_context;
		TCompare _less_than;

		/**
		 * @brief      Constructs a tree
		 *
		 * @param[in]  context  The memory context to use for memory allocation and freeing
		 */
		Red_Black_Tree(const Memory_Context& context = os->global_memory)
			:_root(nullptr), _count(0), mem_context(context)
		{}

		/**
		 * @brief      Constructs a tree
		 *
		 * @param[in]  compare_function  The compare function
		 * @param[in]  context           The memory context
		 */
		Red_Black_Tree(const TCompare& compare_function,
					   const Memory_Context& context = os->global_memory)
			:_root(nullptr), _count(0), mem_context(context),
			 _less_than(compare_function)
		{}

		/**
		 * @brief      Constructs a tree with the provided list
		 *
		 * @param[in]  list              The initializer list to fill the tree with
		 * @param[in]  compare_function  The compare function
		 * @param[in]  context           The memory context
		 */
		Red_Black_Tree(std::initializer_list<T> list,
					   const TCompare& compare_function = TCompare(),
					   const Memory_Context& context = os->global_memory)
			:_root(nullptr), _count(0), mem_context(context), _less_than(compare_function)
		{
			auto it = list.begin();
			for(usize i = 0;
				i < list.size();
				++i)
			{
				insert(*it);
				it = std::next(it);
			}
		}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other  The other tree to copy from
		 */
		Red_Black_Tree(const Red_Black_Tree& other)
			:_root(nullptr), _count(0), mem_context(other.mem_context),
			 _less_than(other._less_than)
		{
			_copy_content(other);
		}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other             The other tree to copy from
		 * @param[in]  compare_function  The compare function
		 */
		Red_Black_Tree(const Red_Black_Tree& other,
					   const TCompare& compare_function)
			:_root(nullptr), _count(0), mem_context(other.mem_context),
			 _less_than(compare_function)
		{
			_copy_content(other);
		}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other    The other tree to copy from
		 * @param[in]  context  The memory context
		 */
		Red_Black_Tree(const Red_Black_Tree& other,
					   const Memory_Context& context)
			:_root(nullptr), _count(0), mem_context(context),
			 _less_than(other._less_than)
		{
			_copy_content(other);
		}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other             The other tree to copy from
		 * @param[in]  context           The memory context
		 * @param[in]  compare_function  The compare function
		 */
		Red_Black_Tree(const Red_Black_Tree& other,
					   const Memory_Context& context,
					   const TCompare& compare_function)
			:_root(nullptr), _count(0), mem_context(context),
			 _less_than(compare_function)
		{
			_copy_content(other);
		}

		/**
		 * @brief      Move Constructor
		 *
		 * @param[in]  other  The other tree to move from
		 */
		Red_Black_Tree(Red_Black_Tree&& other)
			:_root(other._root), _count(other._count),
			 mem_context(std::move(other.mem_context)),
			 _less_than(std::move(other._less_than))
		{
			other._root = nullptr;
			other._count = 0;
		}

		/**
		 * @brief      Destroys the tree.
		 */
		~Red_Black_Tree()
		{
			clear();
		}

		/**
		 * @brief      Copy Assignment operator
		 *
		 * @param[in]  other  The other tree to copy from
		 *
		 * @return     A Reference to this tree
		 */
		Red_Black_Tree&
		operator=(const Red_Black_Tree& other)
		{
			clear();
			mem_context = other.mem_context;
			_less_than = other._less_than;
			_copy_content(other);
			return *this;
		}

		/**
		 * @brief      Move Assignment operator
		 *
		 * @param[in]  other  The other tree to move from
		 *
		 * @return     A Reference to this tree
		 */
		Red_Black_Tree&
		operator=(Red_Black_Tree&& other)
		{
			_reset(_root);
			mem_context = std::move(other.mem_context);
			_less_than = std::move(other._less_than);

			_count = other._count;
			_root = other._root;
			other._count = 0;
			other._root = nullptr;
			return *this;
		}

		/**
		 * @brief      Clears the values in the tree
		 */
		void
		clear()
		{
			_reset(_root);
			_root = nullptr;
			_count = 0;
		}

		/**
		 * @brief      Clears the values in the tree and frees the memory
		 */
		void
		reset()
		{
			clear();
		}

		/**
		 * @brief      Inserts a value into the tree
		 *
		 * @param[in]  data  The data to insert
		 *
		 * @return     an iterator to the inserted value
		 */
		iterator
		insert(const Data_Type& data)
		{
			return iterator(_insert(data));
		}

		/**
		 * @brief      Inserts a value into the tree
		 *
		 * @param[in]  data  The data to insert
		 *
		 * @return     an iterator to the inserted value
		 */
		iterator
		insert(Data_Type&& data)
		{
			return iterator(_insert(std::move(data)));
		}

		/**
		 * @brief      Removes a value from the tree
		 *
		 * @param[in]  data  The data to remove from the tree
		 */
		void
		remove(const Data_Type& data)
		{
			remove(lookup(data));
		}

		/**
		 * @brief      Removes a value from the tree
		 *
		 * @param[in]  data  The data to remove from the tree
		 */
		void
		remove(Data_Type&& data)
		{
			remove(lookup(data));
		}

		/**
		 * @brief      Removes a value from the tree
		 *
		 * @param[in]  node_to_delete  An iterator to the value to be removed
		 */
		void
		remove(iterator node_to_delete)
		{
			if(node_to_delete == end())
				return;

			Node_Type *x = nullptr, *x_parent = nullptr;
			iterator y;

			if (node_to_delete.node->left == nullptr || node_to_delete.node->right == nullptr)
				y = node_to_delete;
			else
			{
				y = node_to_delete;
				++y;
			}

			if (y.node->left != nullptr)
				x = y.node->left;
			else
				x = y.node->right;

			if (x != nullptr)
				x->parent = y.node->parent;

			x_parent = y.node->parent;

			if (y.node->parent == nullptr)
			{
				_root = x;
				if (x != nullptr)
					x->color = Color_Type::BLACK;
			}
			else if (y.node == y.node->parent->left)
				y.node->parent->left = x;
			else
				y.node->parent->right = x;

			if (y != node_to_delete)
			{
				TConstFreeDataType& node_to_delete_data = (TConstFreeDataType&)node_to_delete.node->data;
				TConstFreeDataType& y_node_data = (TConstFreeDataType&)y.node->data;
				node_to_delete_data = y_node_data;
			}

			if (y.node->color == Color_Type::BLACK)
				_rb_delete_fixup(x, x_parent);

			_free_mem(y.node);
		}

		/**
		 * @brief      Looks-up a value in the tree
		 *
		 * @param[in]  data  The data to look-up
		 *
		 * @return     an iterator to the value or to the end of the tree if it doesn't exist
		 */
		iterator
		lookup(const Data_Type& data)
		{
			Node_Type* result = _root;
			if(result != nullptr)
				result = _lookup(data, _root);
			return iterator(result);
		}

		/**
		 * @brief      Looks-up a value in the tree
		 *
		 * @param[in]  data  The data to look-up
		 *
		 * @return     an iterator to the value or to the end of the tree if it doesn't exist
		 */
		iterator
		lookup(Data_Type&& data)
		{
			return lookup(data);
		}

		/**
		 * @brief      Looks-up a value in the tree
		 *
		 * @param[in]  data  The data to look-up
		 *
		 * @return     a const iterator to the value or to the end of the tree if it doesn't exist
		 */
		const_iterator
		lookup(const Data_Type& data) const
		{
			Node_Type* result = _root;
			if(result != nullptr)
				result = _lookup(data, _root);
			return const_iterator(result);
		}

		/**
		 * @brief      Looks-up a value in the tree
		 *
		 * @param[in]  data  The data to look-up
		 *
		 * @return     a const iterator to the value or to the end of the tree if it doesn't exist
		 */
		const_iterator
		lookup(Data_Type&& data) const
		{
			return lookup(data);
		}

		/**
		 * @return     An iterator to the root of the tree
		 */
		iterator
		root()
		{
			return iterator(_root);
		}

		/**
		 * @return     A const iterator to the root of the tree
		 */
		const_iterator
		root() const
		{
			return const_iterator(_root);
		}

		/**
		 * @brief      Traverses the tree in an in-order pattern
		 *
		 * @param[in]  FT      Function to apply to the values
		 * @param      user_data      The user data to pass to the function
		 *
		 * @tparam     function_type  Type of the function to be applied
		 * @tparam     user_type      Type of the user data to be passed
		 */
		template<typename function_type, typename user_type = void>
		void
		inorder_traverse(function_type&& FT, user_type* user_data = nullptr)
		{
			_inorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		/**
		 * @brief      Traverses the tree in a post-order pattern
		 *
		 * @param[in]  FT      Function to apply to the values
		 * @param      user_data      The user data to pass to the function
		 *
		 * @tparam     function_type  Type of the function to be applied
		 * @tparam     user_type      Type of the user data to be passed
		 */
		template<typename function_type, typename user_type = void>
		void
		postorder_traverse(function_type&& FT, user_type* user_data = nullptr)
		{
			_postorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		/**
		 * @brief      Traverses the tree in an pre-order pattern
		 *
		 * @param[in]  FT      Function to apply to the values
		 * @param      user_data      The user data to pass to the function
		 *
		 * @tparam     function_type  Type of the function to be applied
		 * @tparam     user_type      Type of the user data to be passed
		 */
		template<typename function_type, typename user_type = void>
		void
		preorder_traverse(function_type&& FT, user_type* user_data = nullptr)
		{
			_preorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		/**
		 * @brief      Traverses the tree in an in-order pattern
		 *
		 * @param[in]  FT      Function to apply to the values
		 * @param      user_data      The user data to pass to the function
		 *
		 * @tparam     function_type  Type of the function to be applied
		 * @tparam     user_type      Type of the user data to be passed
		 */
		template<typename function_type, typename user_type = void>
		void
		inorder_traverse(function_type&& FT, user_type* user_data = nullptr) const
		{
			_inorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		/**
		 * @brief      Traverses the tree in a post-order pattern
		 *
		 * @param[in]  FT      Function to apply to the values
		 * @param      user_data      The user data to pass to the function
		 *
		 * @tparam     function_type  Type of the function to be applied
		 * @tparam     user_type      Type of the user data to be passed
		 */
		template<typename function_type, typename user_type = void>
		void
		postorder_traverse(function_type&& FT, user_type* user_data = nullptr) const
		{
			_postorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		/**
		 * @brief      Traverses the tree in an pre-order pattern
		 *
		 * @param[in]  FT      Function to apply to the values
		 * @param      user_data      The user data to pass to the function
		 *
		 * @tparam     function_type  Type of the function to be applied
		 * @tparam     user_type      Type of the user data to be passed
		 */
		template<typename function_type, typename user_type = void>
		void
		preorder_traverse(function_type&& FT, user_type* user_data = nullptr) const
		{
			_preorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		/**
		 * @brief      Swaps two trees
		 *
		 * @param      other  The other tree to swap with
		 */
		void
		swap(Red_Black_Tree& other)
		{
			std::swap(_root, other._root);
			std::swap(_count, other._count);
			std::swap(mem_context, other.mem_context);
			std::swap(_less_than, other._less_than);
		}

		/**
		 * @return     An iterator to the minimum value of the tree
		 */
		iterator
		min()
		{
			return iterator(_min());
		}

		/**
		 * @return     A const iterator to the minimum value of the tree
		 */
		const_iterator
		min() const
		{
			return const_iterator(_min());
		}

		/**
		 * @return     An iterator to the maximum value of the tree
		 */
		iterator
		max()
		{
			return iterator(_max());
		}

		/**
		 * @return     A const iterator to the maximum value of the tree
		 */
		const_iterator
		max() const
		{
			return const_iterator(_max());
		}

		/**
		 * @return     The count of values in the tree
		 */
		usize
		count() const
		{
			return _count;
		}

		/**
		 * @return     Whether the tree is empty or not
		 */
		usize
		empty() const
		{
			return _count == 0;
		}

		//Container range interface
		
		/**
		 * @return     Range viewing all the values in the tree
		 */
		Range_Type
		all()
		{
			return Range_Type(min().node, _count);
		}

		/**
		 * @return     Const Range viewing all the values in the tree
		 */
		Const_Range_Type
		all() const
		{
			return Const_Range_Type(min().node, _count);
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Range viewing the specified values in the tree
		 */
		Range_Type
		range(usize start, usize end)
		{
			auto it = min();
			for(usize i = 0; i < start; ++i)
				++it;
			return Range_Type(it.node, end - start);
		}

		/**
		 * @param[in]  start  The start index of the range
		 * @param[in]  end    The end index of the range
		 *
		 * @return     Const range viewing the specified values in the tree
		 */
		Const_Range_Type
		range(usize start, usize end) const
		{
			auto it = min();
			for(usize i = 0; i < start; ++i)
				++it;
			return Const_Range_Type(it.node, end - start);
		}

		//iterator interface

		/**
		 * @return     An Iterator to the beginning of this container
		 */
		iterator
		begin()
		{
			return min();
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		begin() const
		{
			return min();
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		cbegin() const
		{
			return min();
		}

		/**
		 * @return     An Iterator to the end of the container
		 */
		iterator
		end()
		{
			return nullptr;
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		end() const
		{
			return nullptr;
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		cend() const
		{
			return nullptr;
		}

		Node_Type*
		_min() const
		{
			auto m = _root;
			if (m == nullptr)
				return nullptr;

			while (m->left != nullptr)
			{
				m = m->left;
			}
			return m;
		}

		Node_Type*
		_max() const
		{
			auto m = _root;
			if (m == nullptr)
				return nullptr;

			while (m->right != nullptr)
			{
				m = m->right;
			}
			return m;
		}

		bool
		_is_red_black_tree() const
		{
			return _is_rb(_root) != -1;
		}

		Node_Type*
		_insert(const T& data)
		{
			Node_Type* y = nullptr;
			Node_Type* x = _root;

			while (x != nullptr)
			{
				y = x;
				if (_less_than(data, x->data))
					x = x->left;
				else if (_less_than(x->data, data))
					x = x->right;
				else
				{
					return x;
				}
			}
			++_count;
			Node_Type* z = _create_node(data);
			z->parent = y;
			if (y == nullptr)
				_root = z;
			else
			{
				if (_less_than(z->data, y->data))
					y->left = z;
				else
					y->right = z;
			}
			_insert_fixup(z);
			return z;
		}

		Node_Type*
		_insert(T&& data)
		{
			Node_Type* y = nullptr;
			Node_Type* x = _root;

			while (x != nullptr)
			{
				y = x;
				if (_less_than(data, x->data))
					x = x->left;
				else if (_less_than(x->data, data))
					x = x->right;
				else
				{
					return x;
				}
			}
			++_count;
			Node_Type* z = _create_node(std::move(data));
			z->parent = y;
			if (y == nullptr)
				_root = z;
			else
			{
				if (_less_than(z->data, y->data))
					y->left = z;
				else
					y->right = z;
			}
			_insert_fixup(z);
			return z;
		}

		void
		_insert_fixup(Node_Type* z)
		{
			auto is_red = [](Node_Type* p) { return p != nullptr && p->color == Color_Type::RED; };

			while (is_red(z->parent))
			{
				if (z->parent == z->parent->parent->left)
				{
					Node_Type* y = z->parent->parent->right;
					if (is_red(y))
					{ //Casse 1
						z->parent->color = Color_Type::BLACK;
						y->color = Color_Type::BLACK;
						z->parent->parent->color = Color_Type::RED;
						z = z->parent->parent;
					}
					else
					{
						if (z == z->parent->right)
						{ //case 2
							z = z->parent;
							_left_rotation(z);
						}
						//case 3
						z->parent->color = Color_Type::BLACK;
						z->parent->parent->color = Color_Type::RED;
						_right_rotation(z->parent->parent);
					}
				}
				else
				{// right instead of left

					Node_Type* y = z->parent->parent->left;
					if (is_red(y))
					{ //Casse 1
						z->parent->color = Color_Type::BLACK;
						y->color = Color_Type::BLACK;
						z->parent->parent->color = Color_Type::RED;
						z = z->parent->parent;
					}
					else
					{
						if (z == z->parent->left)
						{ //case 2
							z = z->parent;
							_right_rotation(z);
						}
						//case 3
						z->parent->color = Color_Type::BLACK;
						z->parent->parent->color = Color_Type::RED;
						_left_rotation(z->parent->parent);
					}
				}
			}
			_root->color = Color_Type::BLACK;
		}

		void
		_rb_delete_fixup(Node_Type* node, Node_Type* node_parent)
		{
			Node_Type* w = nullptr;
			bool is_node_black = true;
			if (node != nullptr)
				is_node_black = node->color == Color_Type::BLACK;

			while (node != _root && is_node_black)
			{
				if (node == node_parent->left)
				{
					w = node_parent->right;
					if (w->color == Color_Type::RED)
					{	//Case 1
						w->color = Color_Type::BLACK;
						node_parent->color = Color_Type::RED;
						_left_rotation(node_parent);
						w = node_parent->right;
					}
					bool is_w_left_black = true;
					bool is_w_right_black = true;
					if (w->left != nullptr)
						is_w_left_black = w->left->color == Color_Type::BLACK;
					if (w->right != nullptr)
						is_w_right_black = w->right->color == Color_Type::BLACK;

					if (is_w_left_black	&& is_w_right_black)
					{ //Case 2: black sibling with black childern
						w->color = Color_Type::RED;
						node = node_parent;
						node_parent = node->parent;
						is_node_black = node->color == Color_Type::BLACK;
					}
					else
					{
						if (is_w_right_black)
						{	//Case 3
							w->left->color = Color_Type::BLACK;
							w->color = Color_Type::RED;
							_right_rotation(w);
							w = node_parent->right;
						}
						//Case 4
						w->color = node_parent->color;
						node_parent->color = Color_Type::BLACK;
						if (w->right != nullptr)
							w->right->color = Color_Type::BLACK;
						_left_rotation(node_parent);
						node = _root;
						node_parent = nullptr;
					}

				}
				else
				{
					w = node_parent->left;
					if (w->color == Color_Type::RED)
					{	//Case 1
						w->color = Color_Type::BLACK;
						node_parent->color = Color_Type::RED;
						_right_rotation(node_parent);
						w = node_parent->left;
					}

					bool is_w_left_black = true;
					bool is_w_right_black = true;
					if (w->left != nullptr)
						is_w_left_black = w->left->color == Color_Type::BLACK;
					if (w->right != nullptr)
						is_w_right_black = w->right->color == Color_Type::BLACK;

					if (is_w_left_black	&& is_w_right_black)
					{	//Case 2
						w->color = Color_Type::RED;
						node = node_parent;
						node_parent = node->parent;
						is_node_black = node->color == Color_Type::BLACK;
					}
					else
					{
						if (is_w_left_black)
						{	//Case 3
							w->right->color = Color_Type::BLACK;
							w->color = Color_Type::RED;
							_left_rotation(w);
							w = node_parent->left;
						}
						//Case 4
						w->color = node_parent->color;
						node_parent->color = Color_Type::BLACK;
						if (w->left != nullptr)// check this
							w->left->color = Color_Type::BLACK;
						_right_rotation(node_parent);
						node = _root;
						node_parent = nullptr;
					}
				}
			}
			if (node != nullptr)
				node->color = Color_Type::BLACK;
		}

		void
		_left_rotation(Node_Type* x)
		{
			Node_Type* y = x->right;
			x->right = y->left;
			if (y->left != nullptr)
				y->left->parent = x;
			y->parent = x->parent;
			if (x->parent == nullptr)
				_root = y;
			else if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
			y->left = x;
			x->parent = y;
		}

		void
		_right_rotation(Node_Type* x)
		{
			Node_Type* y = x->left;
			x->left = y->right;
			if (y->right != nullptr)
				y->right->parent = x;
			y->parent = x->parent;
			if (x->parent == nullptr)
				_root = y;
			else if (x == x->parent->right)
				x->parent->right = y;
			else
				x->parent->left = y;
			y->right = x;
			x->parent = y;
		}

		Node_Type*
		_create_node(const Data_Type& data)
		{
			Node_Type* result = mem_context.template alloc<Node_Type>().ptr;
			::new (result) Node_Type(data);
			return result;
		}

		Node_Type*
		_create_node(Data_Type&& data)
		{
			Node_Type *result = mem_context.template alloc<Node_Type>().ptr;
			::new (result) Node_Type(std::move(data));
			return result;
		}

		void
		_free_mem(Node_Type* it)
		{
			if (it == nullptr) return;

			it->data.~Data_Type();
			mem_context.free(own(it));
			--_count;
		}

		void
		_free_mem(const Node_Type* it)
		{
			if (it == nullptr) return;

			it->data.~Data_Type();
			mem_context.free(own(it));
			--_count;
		}

		void
		_reset(Node_Type* it)
		{
			if(it == nullptr)
				return;

			auto func = [](iterator it, Red_Black_Tree* tree)
			{
				tree->_free_mem(it.node);
			};
			_postorder_traverse(func, _root, this);
		}

		isize
		_is_rb(Node_Type* it) const
		{
			if (it == nullptr) return 0;

			isize left_count = _is_rb(it->left);
			isize right_count = _is_rb(it->right);

			if (left_count == -1 || right_count == -1 || right_count != left_count)
				return -1;
			else
				return (it->color == Color_Type::RED) ? left_count : left_count + 1;
		}

		void
		_copy_content(const Red_Black_Tree& other)
		{
			if(other.empty())
				return;

			auto func = [](const_iterator it, Red_Black_Tree* tree)
			{
				tree->insert(*it);
			};
			other.preorder_traverse(func, this);
		}

		template<typename function_type, typename user_type>
		void
		_inorder_traverse(function_type&& fT, iterator it, user_type* user_data)
		{
			if (it.node->left != nullptr)
				_inorder_traverse(std::forward<function_type>(fT), it.node->left, user_data);

			fT(it, user_data);

			if (it.node->right != nullptr)
				_inorder_traverse(std::forward<function_type>(fT), it.node->right, user_data);
		}

		template<typename function_type, typename user_type>
		void
		_postorder_traverse(function_type&& fT, iterator it, user_type* user_data)
		{
			if (it.node->left != nullptr)
				_postorder_traverse(std::forward<function_type>(fT), it.node->left, user_data);

			if (it.node->right != nullptr)
				_postorder_traverse(std::forward<function_type>(fT), it.node->right, user_data);

			fT(it, user_data);
		}

		template<typename function_type, typename user_type>
		void
		_preorder_traverse(function_type&& fT, iterator it, user_type* user_data)
		{
			fT(it, user_data);

			if (it.node->left != nullptr)
				_preorder_traverse(std::forward<function_type>(fT), it.node->left, user_data);

			if (it.node->right != nullptr)
				_preorder_traverse(std::forward<function_type>(fT), it.node->right, user_data);
		}

		template<typename function_type, typename user_type>
		void
		_inorder_traverse(function_type&& fT, const_iterator it, user_type* user_data) const
		{
			if (it.node->left != nullptr)
				_inorder_traverse(std::forward<function_type>(fT), it.node->left, user_data);

			fT(it, user_data);

			if (it.node->right != nullptr)
				_inorder_traverse(std::forward<function_type>(fT), it.node->right, user_data);
		}

		template<typename function_type, typename user_type>
		void
		_postorder_traverse(function_type&& fT, const_iterator it, user_type* user_data) const
		{
			if (it.node->left != nullptr)
				_postorder_traverse(std::forward<function_type>(fT), it.node->left, user_data);

			if (it.node->right != nullptr)
				_postorder_traverse(std::forward<function_type>(fT), it.node->right, user_data);

			fT(it, user_data);
		}

		template<typename function_type, typename user_type>
		void
		_preorder_traverse(function_type&& fT, const_iterator it, user_type* user_data) const
		{
			fT(it, user_data);

			if (it.node->left != nullptr)
				_preorder_traverse(std::forward<function_type>(fT), it.node->left, user_data);

			if (it.node->right != nullptr)
				_preorder_traverse(std::forward<function_type>(fT), it.node->right, user_data);
		}

		Node_Type*
		_lookup(const Data_Type& key, Node_Type* it) const
		{
			if (_less_than(key , it->data))
			{
				if (it->left != nullptr)
					return _lookup(key, it->left);

				else
					return it->left;
			}
			else if (_less_than(it->data, key))
			{
				if (it->right != nullptr)
					return _lookup(key, it->right);
				else
					return it->right;
			}
			else
				return it;
		}

		Node_Type*
		_lookup(Data_Type&& key, Node_Type* it) const
		{
			return _lookup(key, it);
		}
	};

	/**
	 * @brief      A Generic Red Black Map
	 *
	 * @tparam     TKey                Key type of pairs in the map
	 * @tparam     TValue              Value type of pairs in the map
	 * @tparam     TCompare            Compare function used by the tree
	 */
	template<typename TKey, typename TValue,
			 typename TCompare = default_less_than<Pair_Node<const TKey, TValue>>>
	struct Red_Black_Map:
		Red_Black_Tree<Pair_Node<const TKey, TValue>, default_less_than<Pair_Node<const TKey, TValue>>,
			internal::Red_Black_Tree_Node<Pair_Node<const TKey, TValue>>,
			typename internal::Red_Black_Tree_Node<Pair_Node<const TKey, TValue>>::Color_Type,
			Red_Black_Tree_Iterator<internal::Red_Black_Tree_Node<Pair_Node<const TKey, TValue>>>,
			Red_Black_Tree_Iterator<const internal::Red_Black_Tree_Node<Pair_Node<const TKey, TValue>>>,
			Pair_Node<TKey, TValue>>
	{
		using Key_Type = TKey;
		using Value_Type = TValue;
		using Data_Type = Pair_Node<const Key_Type, Value_Type>;
		using Node_Type = internal::Red_Black_Tree_Node<Pair_Node<const TKey, TValue>>;
		using Color_Type = typename Node_Type::Color_Type;
		using iterator = Red_Black_Tree_Iterator<internal::Red_Black_Tree_Node<Pair_Node<const TKey, TValue>>>;
		using const_iterator = Red_Black_Tree_Iterator<const internal::Red_Black_Tree_Node<Pair_Node<const TKey, TValue>>>;
		using _impl = Red_Black_Tree<Pair_Node<const TKey, TValue>, default_less_than<Pair_Node<const TKey, TValue>>,
						internal::Red_Black_Tree_Node<Pair_Node<const TKey, TValue>>,
						typename internal::Red_Black_Tree_Node<Pair_Node<const TKey, TValue>>::Color_Type,
						Red_Black_Tree_Iterator<internal::Red_Black_Tree_Node<Pair_Node<const TKey, TValue>>>,
						Red_Black_Tree_Iterator<const internal::Red_Black_Tree_Node<Pair_Node<const TKey, TValue>>>,
						Pair_Node<TKey, TValue>>;

		/**
		 * @brief      Constructs a map
		 *
		 * @param[in]  context  The memory context to use for memory allocation and freeing
		 */
		Red_Black_Map(const Memory_Context& context = os->global_memory)
			:_impl(context)
		{}

		/**
		 * @brief      Constructs a map
		 *
		 * @param[in]  compare_function  The compare function
		 * @param[in]  context           The memory context
		 */
		Red_Black_Map(const TCompare& compare_function,
					  const Memory_Context& context = os->global_memory)
			:_impl(compare_function, context)
		{}

		/**
		 * @brief      Constructs a map with the provided list
		 *
		 * @param[in]  list              The initializer list to fill the map with
		 * @param[in]  compare_function  The compare function
		 * @param[in]  context           The memory context
		 */
		Red_Black_Map(std::initializer_list<Data_Type> list,
					  const TCompare& compare_function = TCompare(),
					  const Memory_Context& context = os->global_memory)
			:_impl(list, compare_function, context)
		{}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other  The other map to copy from
		 */
		Red_Black_Map(const Red_Black_Map& other)
			:_impl(other)
		{}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other             The other map to copy from
		 * @param[in]  compare_function  The compare function
		 */
		Red_Black_Map(const Red_Black_Map& other,
					  const TCompare& compare_function)
			:_impl(other, compare_function)
		{}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other    The other map to copy from
		 * @param[in]  context  The memory context
		 */
		Red_Black_Map(const Red_Black_Map& other,
					  const Memory_Context& context)
			:_impl(other, context)
		{}

		/**
		 * @brief      Copy Constructor
		 *
		 * @param[in]  other             The other map to copy from
		 * @param[in]  context           The memory context
		 * @param[in]  compare_function  The compare function
		 */
		Red_Black_Map(const Red_Black_Map& other,
					  const Memory_Context& context,
					  const TCompare& compare_function)
			:_impl(other, context, compare_function)
		{}

		/**
		 * @brief      Move Constructor
		 *
		 * @param[in]  other  The other map to move from
		 */
		Red_Black_Map(Red_Black_Map&& other)
			:_impl(other)
		{}

		/**
		 * @brief      Subscript operator
		 *
		 * @param[in]  key   The key to search for or insert in the map
		 *
		 * @return     A Reference to the associated value of this key
		 */
		Value_Type&
		operator[](const Key_Type& key)
		{
			return _impl::insert(Data_Type(key))->value;
		}

		/**
		 * @brief      Subscript operator
		 *
		 * @param[in]  key   The key to search for or insert in the map
		 *
		 * @return     A Const Reference to the associated value of this key
		 */
		const Value_Type&
		operator[](const Key_Type& key) const
		{
			return _impl::insert(Data_Type(key))->value;
		}

		/**
		 * @brief      Inserts a key into the map
		 *
		 * @param[in]  key   The key to insert in the map
		 *
		 * @return     an iterator to the inserted pair
		 */
		iterator
		insert(const Key_Type& key)
		{
			return _impl::insert(Data_Type(key));
		}

		/**
		 * @brief      Inserts a key into the map
		 *
		 * @param[in]  key   The key to insert in the map
		 *
		 * @return     an iterator to the inserted pair
		 */
		iterator
		insert(Key_Type&& key)
		{
			return _impl::insert(Data_Type(std::move(key)));
		}

		/**
		 * @brief      Inserts a key value pair into the map
		 *
		 * @param[in]  key    The key to insert in the map
		 * @param[in]  value  The value to insert in the map
		 *
		 * @return     an iterator to the inserted pair
		 */
		iterator
		insert(const Key_Type& key, const Value_Type& value)
		{
			return _impl::insert(Data_Type(key, value));
		}

		/**
		 * @brief      Inserts a key value pair into the map
		 *
		 * @param[in]  key    The key to insert in the map
		 * @param[in]  value  The value to insert in the map
		 *
		 * @return     an iterator to the inserted pair
		 */
		iterator
		insert(Key_Type&& key, const Value_Type& value)
		{
			return _impl::insert(Data_Type(std::move(key), value));
		}

		/**
		 * @brief      Inserts a key value pair into the map
		 *
		 * @param[in]  key    The key to insert in the map
		 * @param[in]  value  The value to insert in the map
		 *
		 * @return     an iterator to the inserted pair
		 */
		iterator
		insert(const Key_Type& key, Value_Type&& value)
		{
			return _impl::insert(Data_Type(key, std::move(value)));
		}

		/**
		 * @brief      Inserts a key value pair into the map
		 *
		 * @param[in]  key    The key to insert in the map
		 * @param[in]  value  The value to insert in the map
		 *
		 * @return     an iterator to the inserted pair
		 */
		iterator
		insert(Key_Type&& key, Value_Type&& value)
		{
			return _impl::insert(Data_Type(std::move(key), std::move(value)));
		}

		using _impl::insert;

		/**
		 * @brief      Removes a key from the map
		 *
		 * @param[in]  key   The key to be removed
		 */
		void
		remove(const Key_Type& key)
		{
			_impl::remove(lookup(Data_Type(key)));
		}

		/**
		 * @brief      Removes a key from the map
		 *
		 * @param[in]  key   The key to be removed
		 */
		void
		remove(Key_Type&& key)
		{
			_impl::remove(lookup(Data_Type(std::move(key))));
		}

		using _impl::remove;

		/**
		 * @brief      Looks-up a key in the map
		 *
		 * @param[in]  key   The key to be looked-up
		 *
		 * @return     An iterator the pair if the key exists otherwise it returns iterator to
		 *             the end of the map
		 */
		iterator
		lookup(const Key_Type& key)
		{
			return _impl::lookup(Data_Type(std::move(key)));
		}

		/**
		 * @brief      Looks-up a key in the map
		 *
		 * @param[in]  key   The key to be looked-up
		 *
		 * @return     An iterator the pair if the key exists otherwise it returns iterator to
		 *             the end of the map
		 */
		iterator
		lookup(Key_Type&& key)
		{
			return _impl::lookup(Data_Type(std::move(key)));
		}

		/**
		 * @brief      Looks-up a key in the map
		 *
		 * @param[in]  key   The key to be looked-up
		 *
		 * @return     A const iterator the pair if the key exists otherwise it returns
		 *             const iterator to the end of the map
		 */
		const_iterator
		lookup(const Key_Type& key) const
		{
			return _impl::lookup(Data_Type(key));
		}

		/**
		 * @brief      Looks-up a key in the map
		 *
		 * @param[in]  key   The key to be looked-up
		 *
		 * @return     A const iterator the pair if the key exists otherwise it returns
		 *             const iterator to the end of the map
		 */
		const_iterator
		lookup(Key_Type&& key) const
		{
			return _impl::lookup(Data_Type(std::move(key)));
		}

		using _impl::lookup;

		/**
		 * @return     An Iterator to the beginning of this container
		 */
		iterator
		begin()
		{
			return _impl::min();
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		begin() const
		{
			return _impl::min();
		}

		/**
		 * @return     A Const iterator to the beginning of this container
		 */
		const_iterator
		cbegin() const
		{
			return _impl::min();
		}

		/**
		 * @return     An Iterator to the end of the container
		 */
		iterator
		end()
		{
			return nullptr;
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		end() const
		{
			return nullptr;
		}

		/**
		 * @return     A Const Iterator to the end of the container
		 */
		const_iterator
		cend() const
		{
			return nullptr;
		}
	};

	/**
	 * [[markdown]]
	 * #Tree_Set
	 * A Tree_Set is a Red_Black_Tree.
	 * 
	 * #Tree_Map
	 * A Tree_Map is a Red_Black_Map
	 */
	template<typename T,
			 typename TCompare = default_less_than<T>>
	using Tree_Set = Red_Black_Tree<T, TCompare>;

	template<typename TKey, typename TValue,
			 typename TCompare = default_less_than<Pair_Node<TKey, TValue>>>
	using Tree_Map = Red_Black_Map<TKey, TValue, TCompare>;
}