#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
#define max_size 256
template<typename t>
class Queue{
private:
	int front, rear, count, cap;
	t*q;
	int next(int i){
		return ((i + 1) % cap);
	}
public:
	Queue(int capacity) :cap(capacity){
		front = 0;
		rear = cap - 1;
		count = 0;
		q = new t[cap];
	}
	void extend_queue(t*&arr, int& old_cap, int new_cap){
		t*temp = new t[new_cap];
		int i = front, j = 0;
		for (; i != rear; i = next(i), j++){
			temp[j] = arr[i];
		}
		temp[j] = arr[i];
		old_cap = new_cap;
		delete[]arr;
		arr = nullptr;
		arr = temp;
	}
	void enque(const t&obj){
		if (count == cap){
			extend_queue(q, cap, cap * 2);
		}
		rear = next(rear);
		q[rear] = obj;
		count++;
	}
	bool is_empty(){
		return (count == 0) ? true : false;
	}
	void shrink_queue(t*&arr, int &old_cap, int new_cap){
		t*temp = new t[new_cap];
		int i = front, j = 0;
		for (; j<new_cap; i = next(i), j++){
			temp[j] = arr[i];
		}
		old_cap = new_cap;
		delete[] arr;
		arr = nullptr;
		arr = temp;
	}
	void dequeue(){
		/*/if (count < (cap / 2)){
		shrink_queue(q, cap, cap / 2);
		}/*/
		if (!is_empty()){
			front = next(front);
			count--;
		}
	}
	const t& peek(){
		return q[front];
	}
};
template <typename T>
class BinaryTree{
protected:
	struct Node{
		T symbol;
		Node*right, *left;
		char bit;
		int bit_no;
		string code;
		Node(const T&obj){
			symbol = obj;
			bit_no = -1;
			bit = '\0';
			code = '\0';
			left = nullptr;
			right = nullptr;
		}
		Node(Node*obj){
			symbol = obj->symbol;
			left = obj->left;
			right = obj->right;
		}
	};
	Node *root;
	int n;
	void In_Order(Node*curr){
		if (curr != nullptr){
			In_Order(curr->left);
			//process
			In_Order(curr->right);
		}
	}
	void Post_Order(Node*curr){
		if (curr != nullptr){
			Post_Order(curr->left);
			Post_Order(curr->right);
			if (curr->bit == '\0' || curr->symbol == '\0'){
				delete curr;
			}
		}
	}
	void Pre_Order(Node*curr){
		if (curr != nullptr){
			//process
			Pre_Order(curr->left);
			Pre_Order(curr->right);
		}
	}
	void level_order_traversal(Node*nptr){
		Queue<Node*> q(max_size);
		if (nptr != nullptr){
			nptr->bit = '\0';
			nptr->symbol = '\0';
			q.enque(nptr);
			int n = 0;
			while (!q.is_empty()){
				Node*curr = q.peek();
				q.dequeue();
				if (curr->left != nullptr){
					curr->left->bit = '0';
					q.enque(curr->left);
				}
				if (curr->right != nullptr){
					curr->right->bit = '1';
					q.enque(curr->right);
				}
			}
		}
	}
	void output_file(ofstream& fout, Node*nptr){
		fout << nptr->symbol << "\t" << nptr->code << endl;
	}
	void assign_code_to_symbol(Node*curr, char*&temp_code, int&i, ofstream& fout){
		if (!is_leaf(curr)){
			if (curr->bit != '\0')
				temp_code[i] = curr->bit;
			assign_code_to_symbol(curr->left, temp_code, i += 1, fout);
			assign_code_to_symbol(curr->right, temp_code, i += 1, fout);
		}
		if (curr->bit != '\0')
			temp_code[i] = curr->bit;
		i += 1;
		temp_code[i] = '\0';
		curr->code = string(temp_code);
		if (curr->symbol != '\0')
			output_file(fout, curr);
		i -= 2;
	}
	bool is_leaf(Node *obj){
		if (obj->right == nullptr&&obj->left == nullptr)
			return true;
		return false;
	}
	bool one_child(Node*nptr){
		if ((nptr->left != nullptr && nptr->right == nullptr) || (nptr->left == nullptr && nptr->right != nullptr))
			return true;
		return false;
	}
	Node* onlychild(Node*nptr){
		if (nptr->left != nullptr)
			return nptr->left;
		else
			return nptr->right;
	}
	void swap(int&a, int&b){
		int	temp = a;
		a = b;
		b = temp;
	}
	int max(int a, int b){
		if (a >= b)
			return a;
		return b;
	}
	bool is_empty(){
		if (n == 0)
			return true;
		return false;
	}
public:
	BinaryTree(){
		root = nullptr;
		n = 0;
	}
	/*/void insert(const T&obj){
	if (is_empty()){
	root = new Node(obj);
	n++;
	}
	else{
	Node*par = nullptr;
	Node*curr = root;
	while (curr != nullptr){
	if (curr->data >= obj){
	par = curr;
	curr = curr->left;
	}
	else{
	par = curr;
	curr = curr->right;
	}
	}
	if (par->data >= obj)
	par->left = new Node(obj);
	else
	par->right = new Node(obj);
	n++;
	}
	}/*/
	void grow_hoffman_tree(Node*nptr){
		if (nptr != nullptr){
			root = new Node(nptr);
			//static int i = 0;
			//cout << i++ << endl;
		}
		else{
			root = nullptr;
		}
	}
	Node* read_tree(Node*curr, int bit){
		(bit == 0)?curr = curr->left:curr = curr->right;
		return curr;
	}
	int height(Node*curr){
		if (curr == nullptr)
			return 0;
		else if (is_leaf(curr))
			return 0;
		else{
			return 1 + max(height(curr->left), height(curr->right));
		}

	}
	int _height(){
		return height(root);
	}
	int fullnodes(Node*nptr){
		if (nptr != nullptr){
			int fleft = fullnodes(nptr->left);
			int fright = fullnodes(nptr->right);
			if (nptr->left != nullptr&&nptr->right != nullptr)
				return(fleft + fright + 1);
			else
				return(fleft + fright);
		}
		else return 0;
	}
	bool remove(const T&key){
		if (is_empty()){
			cout << "There are not any node to remove" << endl;
			return false;
		}
		Node*par = nullptr;
		Node *curr = root;
		while (curr != nullptr&&curr->data->userid != key){
			par = curr;
			if (key < curr->data->userid)
				curr = curr->left;
			else
				curr = curr->right;
		}
		if (curr == nullptr)
			return false;
		bool done = false;
		while (!done){
			if (n == 1){
				delete curr;
				curr = nullptr;
				done = true;
			}
			else if (is_leaf(curr)){
				(par->left == curr) ? par->left = nullptr : par->right = nullptr;
				delete curr;
				curr = nullptr;
				done = true;
			}
			else if (one_child(curr) && par != nullptr){
				(par->left == curr) ? par->left = onlychild(curr) : par->right = onlychild(curr);
				delete curr;
				curr = nullptr;
				done = true;
			}
			else if (one_child(curr) && par == nullptr){
				(curr->left != nullptr) ? par = curr->left : par = curr->right;
				delete curr;
				curr = nullptr;
				root = par;
				done = true;
			}
			else{
				Node*par = curr;
				Node*temp = curr->right;
				while (temp->left != nullptr){
					par = temp;
					temp = temp->left;
				}
				curr->data = temp->data;
				curr = temp;
				if (is_leaf(curr)){
					(par->left == curr) ? par->left = nullptr : par->right = nullptr;
					delete curr;
					curr = nullptr;
					done = true;
				}
				else if (one_child(curr)){
					(par->left == curr) ? par->left = onlychild(curr) : par->right = onlychild(curr);
					delete curr;
					curr = nullptr;
					done = true;
				}
			}
		}
		n--;
		return true;
	}
};
struct Data :public BinaryTree<char>{
	int	frequency;
	Node*type;
	Data(){
		frequency = 0;
		type = nullptr;
	}
	void assign_attributes(int freq, char symb){
		frequency = freq;
		type = new Node(symb);
	}
	void grow_tree(const Data * obj){
		type = obj->type;
		grow_hoffman_tree(obj->type);
	}
	void assign_code(){
		if (root != nullptr){
			level_order_traversal(root);
			int index = -1;
			char*temp=new char[20];
			ofstream fout("Dictionary.txt", ios::out);
			assign_code_to_symbol(root, temp, index, fout);
		}
	}
	void del(){
		Post_Order(root);
	}
	void decode(string temp_str){
		ofstream decoded_text("decoded.txt",ios::out);
		int i = 0;
		bool eof = 0;
		unsigned char a = temp_str[i];
		int bitstream = a, counter = 0, testing_bit = 1, actual_bit = 0;
		while (i<temp_str.size()){
			Node*current_node = root;
			while (current_node->symbol == '\0' || current_node->symbol == '\0'){
				while (counter<8 &&(current_node->symbol == '\0' || current_node->symbol == '\0')){
					int mask = bitstream&testing_bit;
					testing_bit = testing_bit << 1;
					(mask > 0) ? actual_bit = 1 : actual_bit = 0;
					current_node = read_tree(current_node, actual_bit);
					counter++;
				}
				/*/if (counter < 8 && temp_str[i + 1] == '\0'){
					i++;
					break;
				}/*/
				if (counter == 8){
					i++;
					a = temp_str[i];
					if (i == temp_str.size())
						break;
					bitstream = a, counter = 0, testing_bit = 1, actual_bit = 0;
				}
				if (i == temp_str.size())
					break;
			}
			decoded_text << current_node->symbol;
		}
	}
	void _print(){
		//cout << type->symbol << "\t" << frequency << endl;
	}
};
template<typename T>
class Min_Heap :public BinaryTree<char> {
private:
	vector<T> mh;
	inline int lchild(int i){
		return 2 * i;
	}
	inline int rchild(int i){
		return 2 * i + 1;
	}
	inline int parent(int i){
		return i / 2;
	}
	void Min_Heapify(int index){
		if (index > 0 && index < mh.size()){
			int imin = index;
			if (lchild(index) < mh.size() && mh[lchild(index)].frequency<mh[index].frequency)
				imin = lchild(index);
			if (rchild(index) < mh.size() && mh[rchild(index)].frequency < mh[imin].frequency)
				imin = rchild(index);
			if (index != imin){
				_swap(mh[index], mh[imin]);
				Min_Heapify(imin);
			}
		}
	}
	void _swap(Data&obj1, Data&obj2){
		Data*temp = new Data();
		temp->frequency = obj1.frequency;
		temp->type = obj1.type;
		obj1.frequency = obj2.frequency;
		obj1.type = obj2.type;
		obj2.frequency = temp->frequency;
		obj2.type = temp->type;
		delete temp;
	}
public:
	Min_Heap(){
		//waste one space
		T dummy;
		mh.push_back(dummy);
	}
	bool empty(){
		return (mh.size() == 1);
	}
	int insert(const T & obj){
		mh.push_back(obj);
		int index = mh.size() - 1;
		while (index>1 && mh[parent(index)].frequency>mh[index].frequency){
			_swap(mh[parent(index)], mh[index]);
			index = parent(index);
		}
		return index;
	}
	const T& getmin(){
		return mh[1];
	}
	void removeMin(){
		if (mh.size()>1){
			_swap(mh[1], mh[mh.size() - 1]);
			mh.pop_back();
			Min_Heapify(1);
		}
	}
	void buildHeap(const vector<T> & input){
		mh = input;//copy flat array, with dummy character
		//skip the last half of nodes, which will be leaves
		for (int i = mh.size() / 2; i >= 1; i--){
			Min_Heapify(i);
		}
	}
	void assign_data(T const*temp){
		for (int i = 0; i < max_size; i++){
			if (temp[i].frequency > 0){
				if (temp[i].type->symbol != '\0')
					insert(temp[i]);
			}
		}
		buildHeap(mh);
	}
	void build_hoffman_tree(string str){
		int temp_index = 0;
		while (mh.size()>2){
			Data*Dumb = new Data();
			Dumb->assign_attributes(-111, '\0');
			Data*temp = new Data();
			Data temp1 = getmin();
			removeMin();
			Data temp2 = getmin();
			removeMin();
			temp->assign_attributes(temp1.frequency + temp2.frequency, '\0');
			temp_index = insert(*temp);
			Dumb->type->left = temp1.type;
			Dumb->type->right = temp2.type;
			mh[temp_index].grow_tree(Dumb);
			delete Dumb;
			delete temp;
		}
		//mh[temp_index]._print();
		mh[temp_index].assign_code();
		mh[temp_index].decode(str);
		mh[temp_index].del();
	}
	void print_vec(){
		for (int i = 1; i < mh.size(); i++){
			cout << i << ") ";
			mh[i]._print();
		}
	}
	~Min_Heap(){
		for (int i = 0; i < mh.size(); i++){
			delete mh[i].type;
		}
	}
};
class Bitstream:public BinaryTree<char>{
	void calc_no_of_bits(string& temp){
		int i = 0;
		for (; temp[i] != '\0';i++)
		if (i < 8){
			int index = i - 8;
		}
	}
	int calc_power(int x, int y){
		if (y == 0)
			return 1;
		else
		{
			int ans = calc_power(x, y / 2);
			if (y % 2 == 0)
				return ans*ans;
			else
				return ans*ans*x;
		}
	}
	int convert_into_binary(int dec){
		int rem, i = 1, sum = 0;
		do
		{
			rem = dec % 2;
			sum = sum + (i*rem);
			dec = dec / 2;
			i = i * 10;
		} while (dec>0);
		string t_str = to_string(sum);
		if (t_str.size() < 8){
			int occ = 8 - t_str.size();
			t_str.resize(t_str.size()+occ, '0');
			int count = 0;
		}
		sum = stoi(t_str);
		return sum;
	}
	int convert_into_decimal(char* foo){
		int sum = 0;
		for (int i = 0; foo[i] != '\0'; i++){
			int bit = foo[i] - 48;
			sum = sum + bit * calc_power(2, i);
		}
		return sum;
	}
};
void input_freq_table(int*& Frequency){
	for (int i = 0; i < max_size; i++){
		Frequency[i] = 0;
	}
	string temp_Freq = "\0";
	char temp[10];
		ifstream fin("Frequency_table.txt", ios::in);
		if (fin){
			char en = 0;
			int length = 0;
			fin >> length;
			while(!fin.eof()){
				getline(fin,temp_Freq,'\0');
			}
			int char_index = 0;
			int i = 1;
			Frequency[0] = 1;
			for (int no_of_char = 0; no_of_char < length; no_of_char++){
					/*/int j = 0;
					if (no_of_char == 0)
						char_index = i-1;
					else
					char_index = i-2;
					for (; temp_Freq[i] != '\n'; j++, i++){
						temp[j] = temp_Freq[i];
					}
					temp[j] = '\0';
					i += 3;
					char gooch = temp_Freq[char_index];
					int fooch = gooch;
					Frequency[fooch] = stoi(temp);/*/
				char character = '\0';
				if ((i == 1 && temp_Freq[i - 1] == '\t') || (temp_Freq[i] == '\t'&&temp_Freq[i + 1] != '\t')){
					character = '\r';
					i += 1;
				}
				else{
					character = temp_Freq[i];
					i += 2;
				}
				int j = 0;
				for (; temp_Freq[i] != '\n'; i++, j++){
					temp[j] = temp_Freq[i];
				}
				temp[j] = '\0';
				int index = character;
				int freq = stoi(temp);
				Frequency[index] = freq;
				i +=1;
			}
			}
			
		else{
			cout << "File not found" << endl;
		}
		fin.close();
}
void initialize(Data *temp, int*fr_table){
	for (int i = 0; i < max_size; i++){
		temp[i].assign_attributes(fr_table[i], i);
	}
}
void input_compressed_text(string&cmp_txt){
	std::ifstream is("encode.cmp", ios::binary);
	if (is) {
		// get length of file:
		is.seekg(0, is.end);
		int length =is.tellg();
		is.seekg(0, is.beg);
		char * buffer = new char[length+1];
	     is.read(buffer,length);
		buffer[length] = '\0';
		cmp_txt = string(buffer);
		delete[]buffer;
		is.close();
	}

	else{
		cout << "file not found" << endl;
	}
}
int main(){
	int*Freq = new int[max_size];
	input_freq_table(Freq);
	Data *arr = new Data[max_size];
	initialize(arr, Freq);
	string compressed_text = "\0";
	input_compressed_text(compressed_text);
	Min_Heap <Data> abc;
	abc.assign_data(arr);
	abc.build_hoffman_tree(compressed_text);
	delete[]Freq;
	delete[]arr;
	return 0;
}
