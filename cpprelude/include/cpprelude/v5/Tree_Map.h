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

	template<typename T,
			 typename TCompare = default_less_than<T>,
			 typename TNode = internal::Red_Black_Tree_Node<T>,
			 typename TColor = typename TNode::Color_Type,
			 typename TIterator = Red_Black_Tree_Iterator<const TNode>,
			 typename TConstIterator = TIterator,
			 typename TConstFreeDataType = T>
	struct Red_Black_Tree
	{
		using Data_Type = T;
		using Node_Type = TNode;
		using Color_Type = TColor;
		using iterator = TIterator;
		using const_iterator = TConstIterator;

		Node_Type *_root;
		usize _count;
		Memory_Context mem_context;
		TCompare _less_than;

		Red_Black_Tree(const Memory_Context& context = os->global_memory)
			:_root(nullptr), _count(0), mem_context(context)
		{}

		Red_Black_Tree(const TCompare& compare_function,
					   const Memory_Context& context = os->global_memory)
			:_root(nullptr), _count(0), mem_context(context),
			 _less_than(compare_function)
		{}

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

		Red_Black_Tree(const Red_Black_Tree& other)
			:_root(nullptr), _count(0), mem_context(other.mem_context),
			 _less_than(other._less_than)
		{
			_copy_content(other);
		}

		Red_Black_Tree(const Red_Black_Tree& other,
					   const TCompare& compare_function)
			:_root(nullptr), _count(0), mem_context(other.mem_context),
			 _less_than(compare_function)
		{
			_copy_content(other);
		}

		Red_Black_Tree(const Red_Black_Tree& other,
					   const Memory_Context& context)
			:_root(nullptr), _count(0), mem_context(context),
			 _less_than(other._less_than)
		{
			_copy_content(other);
		}

		Red_Black_Tree(const Red_Black_Tree& other,
					   const Memory_Context& context,
					   const TCompare& compare_function)
			:_root(nullptr), _count(0), mem_context(context),
			 _less_than(compare_function)
		{
			_copy_content(other);
		}

		Red_Black_Tree(Red_Black_Tree&& other)
			:_root(other._root), _count(other._count),
			 mem_context(std::move(other.mem_context)),
			 _less_than(std::move(other._less_than))
		{
			other._root = nullptr;
			other._count = 0;
		}

		~Red_Black_Tree()
		{
			clear();
		}

		Red_Black_Tree&
		operator=(const Red_Black_Tree& other)
		{
			clear();
			mem_context = other.mem_context;
			_less_than = other._less_than;
			_copy_content(other);
			return *this;
		}

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

		void
		clear()
		{
			_reset(_root);
			_root = nullptr;
			_count = 0;
		}

		void
		reset()
		{
			clear();
		}

		iterator
		insert(const Data_Type& data)
		{
			return iterator(_insert(data));
		}

		iterator
		insert(Data_Type&& data)
		{
			return iterator(_insert(std::move(data)));
		}

		void
		remove(const Data_Type& data)
		{
			remove(lookup(data));
		}

		void
		remove(Data_Type&& data)
		{
			remove(lookup(data));
		}

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

		iterator
		lookup(const Data_Type& data)
		{
			Node_Type* result = _root;
			if(result != nullptr)
				result = _lookup(data, _root);
			return iterator(result);
		}

		iterator
		lookup(Data_Type&& data)
		{
			return lookup(data);
		}

		const_iterator
		lookup(const Data_Type& data) const
		{
			Node_Type* result = _root;
			if(result != nullptr)
				result = _lookup(data, _root);
			return const_iterator(result);
		}

		const_iterator
		lookup(Data_Type&& data) const
		{
			return lookup(data);
		}

		iterator
		root()
		{
			return iterator(_root);
		}

		const_iterator
		root() const
		{
			return const_iterator(_root);
		}

		template<typename function_type, typename user_type = void>
		void
		inorder_traverse(function_type&& FT, user_type* user_data = nullptr)
		{
			_inorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		template<typename function_type, typename user_type = void>
		void
		postorder_traverse(function_type&& FT, user_type* user_data = nullptr)
		{
			_postorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		template<typename function_type, typename user_type = void>
		void
		preorder_traverse(function_type&& FT, user_type* user_data = nullptr)
		{
			_preorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		template<typename function_type, typename user_type = void>
		void
		inorder_traverse(function_type&& FT, user_type* user_data = nullptr) const
		{
			_inorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		template<typename function_type, typename user_type = void>
		void
		postorder_traverse(function_type&& FT, user_type* user_data = nullptr) const
		{
			_postorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		template<typename function_type, typename user_type = void>
		void
		preorder_traverse(function_type&& FT, user_type* user_data = nullptr) const
		{
			_preorder_traverse(std::forward<function_type>(FT), _root, user_data);
		}

		void
		swap(Red_Black_Tree& other)
		{
			std::swap(_root, other._root);
			std::swap(_count, other._count);
			std::swap(mem_context, other.mem_context);
			std::swap(_less_than, other._less_than);
		}

		iterator
		min()
		{
			return iterator(_min());
		}

		const_iterator
		min() const
		{
			return const_iterator(_min());
		}

		iterator
		max()
		{
			return iterator(_max());
		}

		const_iterator
		max() const
		{
			return const_iterator(_max());
		}

		usize
		count() const
		{
			return _count;
		}

		usize
		empty() const
		{
			return _count == 0;
		}

		iterator
		begin()
		{
			return min();
		}

		const_iterator
		begin() const
		{
			return min();
		}

		const_iterator
		cbegin() const
		{
			return min();
		}

		iterator
		end()
		{
			return nullptr;
		}

		const_iterator
		end() const
		{
			return nullptr;
		}

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
			auto result = mem_context.template alloc<Node_Type>();
			::new (result) Node_Type(data);
			return result;
		}

		Node_Type*
		_create_node(Data_Type&& data)
		{
			auto result = mem_context.template alloc<Node_Type>();
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

		Red_Black_Map(const Memory_Context& context = os->global_memory)
			:_impl(context)
		{}

		Red_Black_Map(const TCompare& compare_function,
					  const Memory_Context& context = os->global_memory)
			:_impl(compare_function, context)
		{}

		Red_Black_Map(std::initializer_list<Data_Type> list,
					  const TCompare& compare_function = TCompare(),
					  const Memory_Context& context = os->global_memory)
			:_impl(list, compare_function, context)
		{}

		Red_Black_Map(const Red_Black_Map& other)
			:_impl(other)
		{}

		Red_Black_Map(const Red_Black_Map& other,
					  const TCompare& compare_function)
			:_impl(other, compare_function)
		{}

		Red_Black_Map(const Red_Black_Map& other,
					  const Memory_Context& context)
			:_impl(other, context)
		{}

		Red_Black_Map(const Red_Black_Map& other,
					  const Memory_Context& context,
					  const TCompare& compare_function)
			:_impl(other, context, compare_function)
		{}

		Red_Black_Map(Red_Black_Map&& other)
			:_impl(other)
		{}

		Value_Type&
		operator[](const Key_Type& key)
		{
			return _impl::insert(Data_Type(key))->value;
		}

		const Value_Type&
		operator[](const Key_Type& key) const
		{
			return _impl::insert(Data_Type(key))->value;
		}

		iterator
		insert(const Key_Type& key)
		{
			return _impl::insert(Data_Type(key));
		}

		iterator
		insert(Key_Type&& key)
		{
			return _impl::insert(Data_Type(std::move(key)));
		}

		iterator
		insert(const Key_Type& key, const Value_Type& value)
		{
			return _impl::insert(Data_Type(key, value));
		}

		iterator
		insert(Key_Type&& key, const Value_Type& value)
		{
			return _impl::insert(Data_Type(std::move(key), value));
		}

		iterator
		insert(const Key_Type& key, Value_Type&& value)
		{
			return _impl::insert(Data_Type(key, std::move(value)));
		}

		iterator
		insert(Key_Type&& key, Value_Type&& value)
		{
			return _impl::insert(Data_Type(std::move(key), std::move(value)));
		}

		using _impl::insert;

		void
		remove(const Key_Type& key)
		{
			_impl::remove(lookup(Data_Type(key)));
		}

		void
		remove(Key_Type&& key)
		{
			_impl::remove(lookup(Data_Type(std::move(key))));
		}

		using _impl::remove;

		iterator
		lookup(const Key_Type& key)
		{
			return _impl::lookup(Data_Type(std::move(key)));

		}

		iterator
		lookup(Key_Type&& key)
		{
			return _impl::lookup(Data_Type(std::move(key)));
		}

		const_iterator
		lookup(const Key_Type& key) const
		{
			return _impl::lookup(Data_Type(key));
		}

		const_iterator
		lookup(Key_Type&& key) const
		{
			return _impl::lookup(Data_Type(std::move(key)));
		}

		using _impl::lookup;

		iterator
		begin()
		{
			return _impl::min();
		}

		const_iterator
		begin() const
		{
			return _impl::min();
		}

		const_iterator
		cbegin() const
		{
			return _impl::min();
		}

		iterator
		end()
		{
			return nullptr;
		}

		const_iterator
		end() const
		{
			return nullptr;
		}

		const_iterator
		cend() const
		{
			return nullptr;
		}
	};

	template<typename T,
			 typename TCompare = default_less_than<T>>
	using Tree_Set = Red_Black_Tree<T, TCompare>;

	template<typename TKey, typename TValue,
			 typename TCompare = default_less_than<Pair_Node<TKey, TValue>>>
	using Tree_Map = Red_Black_Map<TKey, TValue, TCompare>;
}