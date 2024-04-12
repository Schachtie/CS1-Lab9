#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType* m_pRecord;
	struct HashType* m_pNextNode;
};

// Compute the hash function
int hash(int x)
{
	return x % 23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		if (pHashArray[i].m_pRecord != NULL) {
			printf("Index %d: ", i);
			struct HashType* tempHashNode = &pHashArray[i];
			do {
				printf("-> %d, %c, %d ", tempHashNode->m_pRecord->id, tempHashNode->m_pRecord->name, tempHashNode->m_pRecord->order);
				tempHashNode = tempHashNode->m_pNextNode;
			} while (tempHashNode != NULL);
			puts("");
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation

	//Create a hashtable (*) (size 23 to produce collisions)
	struct HashType* hashTable = (struct HashType*)malloc(23 * sizeof(struct HashType));
	for (int i = 0; i < 23; ++i) {
		hashTable[i].m_pRecord = NULL;
		hashTable[i].m_pNextNode = NULL;
	}

	//Read data from pRecords into hashTable
	for (int i = 0; i < recordSz; ++i) {
		//Get key and associated "slot" for each record
		int hashKey = hash(pRecords[i].id);
		struct HashType* tempHashNode = &hashTable[hashKey];

		//Check for collision
		if (tempHashNode->m_pRecord != NULL) {
			//Iterate as much as necessary
			while (tempHashNode->m_pNextNode != NULL) {
				tempHashNode = tempHashNode->m_pNextNode;
			}
			//Create a new node and set values appropriately
			tempHashNode->m_pNextNode = (struct HashType*)malloc(sizeof(struct HashType));
			tempHashNode = tempHashNode->m_pNextNode;
			tempHashNode->m_pNextNode = NULL;
		}
		tempHashNode->m_pRecord = &pRecords[i];
	}

	//Print data from hashTable
	displayRecordsInHash(hashTable, 23);
}