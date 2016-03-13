#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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
		if (count==cap){
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
			//checking if Node is either dummy or Root so delete it
			if (curr->bit == '`' || curr->symbol == '`'){
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
			//assigning dummy node for own convenience
			nptr->bit = '`';
			q.enque(nptr);
			int n = 0;
			while (!q.is_empty()){
				Node*curr = q.peek();
				q.dequeue();
				//if node is in left side
				if (curr->left != nullptr){
					curr->left->bit = '0';
					q.enque(curr->left);
				}
				//if node is in right side
				if (curr->right != nullptr){
					curr->right->bit = '1';
					q.enque(curr->right);
				}
			}
		}
	}
	void output_file(ofstream& fout,Node*nptr){
		fout << nptr->symbol << "\t" << nptr->code<< endl;
	}
	void assign_code_to_symbol(Node*curr, char*temp_code, int&i,ofstream& fout){
		//using huffman algorithm to assign code
		if (!is_leaf(curr)){
			//checking if current node is not root node then assign code
			if (curr->bit != '`')
				temp_code[i] = curr->bit;//collecting every bit in a string for particular character
			assign_code_to_symbol(curr->left, temp_code, i += 1,fout);
			assign_code_to_symbol(curr->right, temp_code, i += 1,fout);
		}
		if (curr->bit != '`')
			temp_code[i] = curr->bit;
		i += 1;
		temp_code[i] = '\0';
		curr->code = string(temp_code);
		if (curr->symbol != '`')//if current node is not dummy node then out put it in a file
			output_file(fout,curr);
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
			//saving each node in a tree
			root = new Node(nptr);
			//static int i = 0;
			//cout << i++ << endl;
		}
		else{
			root = nullptr;
		}
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
		//sending each node to Binary tree
		grow_hoffman_tree(obj->type);
	}
	void assign_code(){
		if (root != nullptr){
			level_order_traversal(root);
			int index = -1;
			char temp[20];
			ofstream fout("Dictionary.txt", ios::out);
			assign_code_to_symbol(root, temp, index, fout);
		}
	}
	void del(){
		//deleteing each new allocated node from tree
		Post_Order(root);
	}
	void _print(){
	//cout << type->symbol << "\t" << frequency << endl;
	}
};
template<typename T>
class Min_Heap:public BinaryTree<char> {
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
		//neglecting null character
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
				if(temp[i].type->symbol != '\0')
				insert(temp[i]);
			}
		}
		buildHeap(mh);
	}
	void build_hoffman_tree(){
		int temp_index = 0;
		while (mh.size()>2){
			Data*Dumb = new Data();
			//assigning dumb node
			Dumb->assign_attributes(-111, '`');
			Data*temp = new Data();
			//getting node with minimum freq from min heap
			Data temp1 = getmin();
			//removing that used minimum from min heap
			removeMin();
			//getting second minimum
			Data temp2 = getmin();
			//removing second minimum
			removeMin();
			// making dumb node parent of two minimum nodes
			temp->assign_attributes(temp1.frequency + temp2.frequency, '`');
			//inserting and getting that index at which node is inserted
			temp_index = insert(*temp);
			Dumb->type->left = temp1.type;
			Dumb->type->right = temp2.type;
			//growing tree from that index
			mh[temp_index].grow_tree(Dumb);
			delete Dumb;
			delete temp;
		}
		//mh[temp_index]._print();
		mh[temp_index].assign_code();
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
class Bitstream{
private:
	string encoded_text;
public:
	Bitstream(string enc_txt) :encoded_text(enc_txt){
		//packing 8 bits
		ofstream out("encode.cmp", ios::out);
		char*temp_str=new char[8];
		for (int i = 0; i < enc_txt.size();){
			int j = 0;
			//separating eight bits
			for (; j < 8&&i<enc_txt.size(); j++){
				temp_str[j] = enc_txt[i];
				i++;
			}
			temp_str[j] = '\0';
			int decimal = 0;
			//converting into its ascii
			decimal=convert_into_decimal(temp_str);
		   //giving that ascii in character
			char byte_offset = decimal;
			out << byte_offset;
		}
		out.close();
		temp_str = nullptr;
		delete[]temp_str;
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
	int convert_into_decimal(char* foo){
		int sum = 0;
		for (int i = 0; foo[i] != '\0'; i++){
			int bit = foo[i] - 48;
			sum = sum + bit * calc_power(2, i);
		}
		return sum;
	}
};
void input_file(string& text,int&lof){
	string filename;
	cout << "Enter filename: ";
	cin >> filename;
	/*/if (fin.is_open()){
		while (!fin.eof()){
			getline(fin, text, '\0');
		}
	}/*/
	std::ifstream fin(filename, ios::binary);
	if (fin) {
		// get length of file:
		fin.seekg(0, fin.end);
		int length = fin.tellg();
		lof = length;
		fin.seekg(0, fin.beg);
		char * buffer = new char[length + 1];
		fin.read(buffer, length);
		buffer[length] = '\0';
		text = string(buffer);
		//cout << cmp_txt << endl;
		delete[]buffer;
		fin.close();
	}
	else{
		cout << "File not found " << endl;
		input_file(text,lof);
	}
}
void calc_freq(string temp,static int*&temp_freq){
	for (int i = 0; i < max_size; i++)
		temp_freq[i] = 0;
	int index = 0;
	int i = 0;
	for (; i<temp.size(); i++){
		index = temp[i];
		temp_freq[index]++;
	}
	index = temp[i];
	temp_freq[index]++;
	int count = 0;
	ofstream freq("Frequency_table.txt", ios::out);
	for (int i = 1; i < max_size; i++){
		char a = i;
		if (temp_freq[i]>=1)
		count++;
	}
	freq << count << endl;
	for (int i = 1; i < max_size; i++){
		char a = i;
		if (temp_freq[i] >= 1)
			freq << a << "\t" << temp_freq[i] << endl;
	}
}
void calc_file_size(int*frequency){
	double sum = 0;
	for (int i = 0; i < max_size; i++){
		sum = sum + frequency[i];
	}
	sum = sum * 8;
	sum /= 8;
	double kb = sum / 1024;
	kb += 0.04;
	cout << "filesize = " << kb << " KB" << endl;
}
void initialize(Data *temp, int*fr_table){
	for (int i = 0; i < max_size; i++){
		temp[i].assign_attributes(fr_table[i], i);
	}
}
void encode_text(string txt,string&enc_txt){
	string ex;
	const int size = txt.size() * 20;
/*/	char *check=new char[size];
	int kl = 0;
	for (int i = 0; i < txt.size();i++){
		for (int j = 0; j < 8; j++){
			check[kl] = '0';
			kl++;
		}
	}
	check[kl] = '\0';/*/
	char *temp=new char[size];
	ifstream fin("Dictionary.txt", ios::in);
	if (fin.is_open()){
		while (!fin.eof()){
			getline(fin, ex,'\0');
		}
		fin.close();
	}
	else{
		cout << "file not found" << endl;
	}
	int j = 0;
	for (int i = 0;i<txt.size(); i++){
		char foo = txt[i];
		for (int k = 0;k<ex.size(); k++){
			if (foo == ex[k]&&ex[k+1]=='\t'){
				k += 2;
				for (; ex[k] != '\n';k++){
					temp[j] = ex[k];
					j++;
				}
				break;
			}
		}
     }
	temp[j] = '\0';
	enc_txt = string(temp);
	//string extra = string(check);
	delete[]temp;
	//delete[]check;
	//out<< extra << endl;
}
int main(/*/int argc,char*argv[]/*/){
	int lof = 0;
	string temp_arr;
	input_file(temp_arr,lof);
	if (lof != 0){
		static int*freq_table = new int[max_size];
		calc_freq(temp_arr, freq_table);
		Data *lmn = new Data[max_size];
		initialize(lmn, freq_table);
		Min_Heap <Data> abc;
		abc.assign_data(lmn);
		abc.build_hoffman_tree();
		string encoded_text = "\0";
		encode_text(temp_arr, encoded_text);
		Bitstream offset(encoded_text);
		//abc.print_vec();
		//calc_file_size(freq_table);
		delete[]freq_table;
		delete[]lmn;
	}
	return 0;
}