#include "invertedindex.h"

template<typename T1, typename T2>
void func_merge(std::map<T1, std::vector<T2>>& map1, const std::map<T1, std::vector<T2>>& map2){
    for (const auto&[key, vect]:map2){
        map1[key].insert(map1[key].cend(), vect.begin(), vect.end());
    }
}

std::string InvertedIndex::ReadDocument(const std::string &fileName) {
    std::string currWord, currTextDocument;
    int counterWords = 0;
    int counterChar = 0;
    bool isTyping = false;
std::fstream file;
    file.open(fileName, std::ios::in);
    if (!file.is_open()) {
        std::cout << "Unable to open file " << fileName << std::endl;
    }else {
        // полчим размер файла
        file.seekg(0, std::ios::end);
        std::streampos lengthFile = file.tellg();
        file.seekg(0, std::ios::beg);
        // дальше будем что-то делать
        std::vector<char>  fileBuff(lengthFile);
        file.read(&fileBuff[0], lengthFile);
        file.close();

        for (auto ptrc=fileBuff.begin(); ptrc != fileBuff.end(); ++ptrc){
            if ((*ptrc >= 'A' && *ptrc <= 'Z') || (*ptrc >= 'a' && *ptrc <= 'z')) {
                if(++counterChar <= mMaxCharInWord) {
                    currTextDocument += tolower(*ptrc);
                }else if (!isTyping) {
                    std::cerr << "WARNING: count of word in file " << fileName << " greater then "
                              << mMaxCharInWord << std::endl;
                    isTyping = true;
                }
            }

            if (counterWords > mMaxWordInDocument) {
                std::cerr << "WARNING: count of word in file " << fileName << " greater then "
                          << mMaxWordInDocument << std::endl;
                break;
            }

            if (*ptrc == ' ') {
                if ((ptrc+1) != fileBuff.end() && *(ptrc + 1) != ' ') {
                    currTextDocument += ' ';
                    counterWords++;
                    counterChar = 0; isTyping = false;
                }
            }
        }
    }
        return currTextDocument;
}

void InvertedIndex::ToIndexDoc(const size_t &docId, const std::string &docFileName) {
    std::map<std::string, std::vector<Entry>> miniFreqDictionary;
    std::vector<std::string> words =  SplitIntoWords(ReadDocument(docFileName));

//TODO подумать над тем что docId поидее будет один и тот же, или что-то должно упроститься. мне так кажется
    for (const std::string &word: words) {
        if (!miniFreqDictionary.count(word)) { // если такого слова в словаре ещё нет
            miniFreqDictionary[word].push_back({docId, 1}); // создаём запись с таким словом
        } else {
            if (miniFreqDictionary[word].back().docId == docId) { // если такое слово в словаре уже есть и относится к текущему документу
                miniFreqDictionary[word].back().count++;  // увеличиваем количество вхождений на 1
            } else {
                miniFreqDictionary[word].push_back({docId, 1}); // если это слово в текущем документе встретилось впервые, то делаем запись с 1.
            }
        }
    }
//А тепеь займёмся перенесением полученных результатов в общий котёл
    if (mLockDictionary!= nullptr ){ // если нет указателя, значит mutex не нужен
        mLockDictionary->lock(); // лочим общий котёл до лучших времён
        mAllDocLengthInWord += words.size();
        mDocLength[docId] = words.size();
        func_merge(mFreqDictionary, miniFreqDictionary); // произведём слияние словарей
        mLockDictionary->unlock();
    } else {
        mAllDocLengthInWord += words.size();
        mDocLength[docId] = words.size();
        func_merge(mFreqDictionary, miniFreqDictionary); // произведём слияние словарей
    }

}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string> &fileNames) {
    
    if (fileNames.empty()){
           std::cerr << "WARNING: List of filenames is empty." << std::endl;
            return;
    }
    mAllDocLengthInWord = 0;
    /** заполненение частотного словаря */
    size_t docId = 0;
        for (const auto &fileName:fileNames) {
        ToIndexDoc(docId, fileName);
            docId++;
        }

    CalcalateAVGDL();
    CalculateIDF();
}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string> &fileNames, const int &maxThreads) {
    //mDocs = std::vector<std::string>(inputDocs.begin(), inputDocs.end()); Ничего записывать в mDocs не будем
    mLockDictionary = new std::mutex;
    if (fileNames.empty()){
           std::cerr << "WARNING: List of filenames is empty." << std::endl;
            return;
    }

    mAllDocLengthInWord = 0;
    synced_stream sync_out;
thread_pool pool;
    int _hWMaxThreads = std::thread::hardware_concurrency();
    if (_hWMaxThreads < maxThreads){
        std::cout << "WARNING: to many thread " << std::endl;
        std::cout << "Hardware " << _hWMaxThreads <<" threads." << std::endl;
    }
pool.reset(_hWMaxThreads);
//TODO проверить в данном случае требуется ли ограничение по количеству потоков, и если да то на каком
    
    mAllDocLengthInWord = 0;
    /** заполненение частотного словаря */
    size_t docId = 0;
        for (const auto &fileName:fileNames) {
        pool.submit([this, docId, fileName, &sync_out] { ToIndexDoc(docId, fileName);
        sync_out.println("Docs : ", docId, " processed ");
        });
        
        docId++;
        }

    pool.wait_for_tasks();

   delete mLockDictionary;
   mLockDictionary = nullptr;

    CalcalateAVGDL();
    CalculateIDF();
}
 
std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word){
    if (mFreqDictionary.count(word)){
        return mFreqDictionary.at(word);
    } else {
        return std::vector<Entry>({});
    }

}

size_t InvertedIndex::GetWordCountAllDocs(const std::string& word){

    if (mFreqDictionary.count(word)){ //  если слово есть в словаре, то считаем его вхождение во все документы
        int result=0;
        for (const auto &entryArray:mFreqDictionary.at(word)){
            result += entryArray.count;
        }
        return result; // и возвращаем резульат подсчёта
    } else {
        return 0; // если слова нет, то встречается оно 0 раз.
    }

}

void InvertedIndex::CalculateIDF(){
    size_t N = GetDocCount();
    for (const auto &docFreq:mFreqDictionary){
        std::string currentWord = docFreq.first;
        size_t  nqi = docFreq.second.size(); //количество документов в которых есть слово
        double IDF = std::log(1.0 * N / nqi);
//        float IDF = std::log10f((1.0 * N - 1.0*nqi+0.5 )/( 1.0*nqi+0.5));
         mIDF.insert(std::pair(currentWord, IDF));
    }
}

void InvertedIndex::CalcalateAVGDL(){
    mAVGDL = 1.f*mAllDocLengthInWord /GetDocCount();
}

double InvertedIndex::GetAVGDL(){
    return  mAVGDL;
};

double InvertedIndex::GetIDF(const std::string &word){
    if (mIDF.count(word)){
        return mIDF.at(word);
    } else {
        return 0;
    }
}

size_t InvertedIndex::GetDocLength(const size_t &docID){
    if (mDocLength.count(docID)) {
        return mDocLength.at(docID);
    }else {
        return -1;
    }
}

size_t InvertedIndex::GetDocCount(){
    return mDocLength.size(); // количество документов ко
}

int InvertedIndex::GetWordCountInDoc(const std::string& word, size_t docId){
    for (const auto &entry:GetWordCount(word)){
        if (entry.docId == docId) return entry.count;
    }
    return 0;
}

void InvertedIndex::SetMaxWordInDocument(int _MaxWordInDocument){
   if (_MaxWordInDocument <= 0) {
        std::cerr << "WARNING! Maximum word in document must greater then 0" << std::endl;
        std::cout << "Set maximum word in document is 1000" << std::endl;
        mMaxWordInDocument = 1000;
    }
    mMaxWordInDocument = _MaxWordInDocument;
};

void InvertedIndex::SetMaxCharInWord(int _MaxCharInWord){
       if (_MaxCharInWord <= 0) {
        std::cerr << "WARNING! Maximum character in word must greater then 0" << std::endl;
        std::cout << "Set maximum character in word is 100" << std::endl;
        mMaxWordInDocument = 1000;
    }
    mMaxWordInDocument = _MaxCharInWord;
};