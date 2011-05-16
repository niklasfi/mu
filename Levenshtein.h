class Levenshtein{
	public:
	unsigned int* data;
	unsigned int** matrix;

	Levenshtein(vector<int>* reference, vector<int>* guess);//die Referenzübersetzung und die Übersetzung in ID's sind 
	~Levenshtein;
	
	void print();//gibt die nötigen Veränderungen von guess auf reference aus.

	int distance(); //gibt die Levenshtein-Distance aus
}
