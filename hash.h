
#include<iomanip>
#include<iostream>
#include<string>
#include<vector>

using namespace std;


template <class HashedObj>
class HashTable
{
public:
	explicit HashTable(const HashedObj & notFound, int size = 101);
	HashTable(const HashTable & rhs) : currentSize(rhs.currentSize), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), array(rhs.array) {}

	const HashedObj & find(const HashedObj & x) const;
	void makeEmpty();
	void insert(const HashedObj & x);
	void remove(const HashedObj & x);
	void print();
	void create(int size);
	const HashedObj & findarray(const HashedObj & x) const;

	const HashTable & operator=(const HashTable & rhs);

	enum EntryType { ACTIVE, EMPTY, DELETED };

private:


	struct HashEntry
	{
		HashedObj element;
		EntryType info;

		HashEntry(const HashedObj & e = HashedObj(), EntryType i = EMPTY) : element(e), info(i) {}
	};

	vector<HashEntry> array;
	int currentSize;
	const HashedObj ITEM_NOT_FOUND;
	int nextPrime(int size);
	int previousPrime(int size) const;
	bool isActive(int currentPos) const;
	int findPos(const HashedObj & x) const;
	void rehash();
	int hash(const string & key, int tableSize) const;
	int hash(int key, int tableSize) const;
	void print(int tableSize) const;

};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class HashedObj>
void HashTable<HashedObj>::create(int size)
{
	array.resize(size);
	makeEmpty();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class HashedObj>
void HashTable<HashedObj>::print()
{

	print(array.size());

}

template <class HashedObj>
void HashTable<HashedObj>::print(int tableSize) const
{
	cout << "\t------------------------\n\t" << setw(6) << "[ADDR][" << setw(7) << "ELEMENT][" << setw(9) << "INFO]\n";
	for (int i = 0; i<tableSize; i++)
	{
		string info = array[i].info == 0 ? "ACTIVE" : array[i].info == 1 ? "EMPTY" : "DELETED";
		cout << "\t------------------------\n\t" << "[" << setw(4) << i << "][" << setw(7) << array[i].element << "][" << setw(7) << info << "]\n";
	}
	cout << "\t------------------------\n";
}

template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj & notFound, int size) : ITEM_NOT_FOUND(notFound), array(nextPrime(size))
{
	makeEmpty();
}

////////////////////////////////////////////////////////

template<class HashedObj>
void HashTable<HashedObj>::makeEmpty()
{
	currentSize = 0;
	for (int i = 0; i<array.size(); i++)
		array[i].info = EMPTY;

}

////////////////////////////////////////////////////////

template<class HashedObj>
const HashedObj & HashTable<HashedObj>::find(const HashedObj & x) const
{
	int currentPos = findPos(x);
	return isActive(currentPos) ? array[currentPos].element : ITEM_NOT_FOUND;
}
////////////////////////////////////////////////////////

template<class HashedObj>
const HashedObj & HashTable<HashedObj>::findarray(const HashedObj & x) const
{
	int currentPos = findPos(x);
	int index = isActive(currentPos) ? currentPos : ITEM_NOT_FOUND;
	if (index != ITEM_NOT_FOUND) cout << "\n\t!!! ADDRESS OF " << x << " IN ARRAY INDEX IS " << index << " !!!\n\t!!! AND ADDRESS OF MEMORY IS " << &array[index] << " !!!\n\n";
	return index;
}

////////////////////////////////////////////////////////

template<class HashedObj>
int HashTable<HashedObj> ::findPos(const HashedObj & x) const
{
	int collisionNum = 0;
	int currentPos = hash(x, array.size());
	int R = previousPrime(array.size());
	int temp = currentPos;
	

	while (array[temp].info != EMPTY&&array[temp].element != x)
	{
		temp = (currentPos + (++collisionNum * (R - (x % R)))) % array.size();
	}

	return temp;
}

////////////////////////////////////////////////////////

template<class HashedObj>
bool HashTable<HashedObj>::isActive(int currentPos) const
{
	return array[currentPos].info == ACTIVE;
}

////////////////////////////////////////////////////////

template<class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj & x)
{
	int currentPos = findPos(x);
	if (isActive(currentPos))
		return;
	array[currentPos] = HashEntry(x, ACTIVE);

	if (++currentSize>array.size()*0.7)
		rehash();

}

////////////////////////////////////////////////////////

template<class HashedObj>
void HashTable<HashedObj>::remove(const HashedObj & x)
{
	int currentPos = findPos(x);
	if (isActive(currentPos))
		array[currentPos].info = DELETED;
	cout << "\tCopy Right By kingkong64" << endl;
}

////////////////////////////////////////////////////////

template <class HashedObj>
void HashTable<HashedObj>::rehash()
{
	vector<HashEntry> oldArray = array;

	array.resize(nextPrime(2 * oldArray.size()));
	for (int j = 0; j<array.size(); j++)
		array[j].info = EMPTY;

	currentSize = 0;
	for (int i = 0; i<oldArray.size(); i++)
		if (oldArray[i].info == ACTIVE)
			insert(oldArray[i].element);
}

////////////////////////////////////////////////////////

template <class HashedObj>
int HashTable<HashedObj>::nextPrime(int size)
{
	int i = 2;

	while (size%i != 0) i++;
	if (i == size) return i;
	else{
		i = size + 1;
		bool check = false;
		while (true){
			for (int j = 2; j <= i; j++){
				if (i%j == 0 && j<i) break;
				else if (i%j == 0 && j == i) check = true;
			}
			if (check) break;
			i++;
		}
		return i;
	}
}


////////////////////////////////////////////////////////

template <class HashedObj>
int HashTable<HashedObj>::previousPrime(int size) const
{
	int R = size;
	bool flag = true;
	while (flag){
		R--;
		if (R % 2 != 0){
			for (int i = R - 1; i >= 2; i--){
				if (R%i == 0){
					break;
				}
				if (i == 2 && R%i != 0){
					flag = false;
				}
			}
		}
	}
	return R;
}

////////////////////////////////////////////////////////

template<class HashedObj>
int HashTable<HashedObj>::hash(const string & key, int tableSize) const
{
	int hashVal = 0;
	for (int i = 0; i<key.length(); i++)
		hashVal += key[i];

	return hashVal % tableSize;

}

template<class HashedObj>
int HashTable<HashedObj>::hash(int key, int tableSize) const
{
	if (key < 0) key = -key;
	return key % tableSize;
}
