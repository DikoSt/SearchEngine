#include "invertedindex.h"

template<typename T1, typename T2>
void func_merge(std::map<T1, std::vector<T2>>& map1, const std::map<T1, std::vector<T2>>& map2){
    for (const auto&[key, vect]:map2){
        map1[key].insert(map1[key].cend(), vect.begin(), vect.end());
    }
}

void InvertedIndex::ToIndexDoc(const size_t &docId, const std::string &textDocument) {
//TODO пересмотреть, хоть и работает но логка мне не очень нравится

    std::map<std::string, std::vector<Entry>> miniFreqDictionary;
    std::vector<std::string> words =  SplitIntoWords(textDocument);

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

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string> &inputDocs) {
    mDocs = std::vector<std::string>(inputDocs.begin(), inputDocs.end());
    if (mDocs.empty()) return;

    mAllDocLengthInWord = 0;
    /** заполненение частотного словаря */
    size_t docId = 0;
        for (const auto &textDocument:mDocs) {
        ToIndexDoc(docId, textDocument);
            docId++;
        }

    CalcalateAVGDL();
    CalculateIDF();
}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string> &inputDocs, const int &maxThreads) {
    mDocs = std::vector<std::string>(inputDocs.begin(), inputDocs.end());
    if (mDocs.empty()) return;
    mAllDocLengthInWord = 0;

    int _hWMaxThreads = std::thread::hardware_concurrency();
    if (_hWMaxThreads < maxThreads){
        std::cout << "WARNING: to many thread " << std::endl;
        std::cout << "Create only " << _hWMaxThreads <<" threads." << std::endl;
    } else _hWMaxThreads = maxThreads;

    mLockDictionary = new std::mutex;
    //std::vector<std::thread> indexThread(maxThreads);
    std::vector<std::thread> indexThread(_hWMaxThreads);

    /** заполненение частотного словаря */
    size_t docId = 0;
    auto iter = mDocs.begin();
    auto textDocument = *iter;

    while (iter != mDocs.cend()) {
        for (int i = 0; i < _hWMaxThreads; ++i) {
            textDocument = *iter;
            indexThread[i] = std::thread([this, docId, textDocument] { ToIndexDoc(docId, textDocument);});
            docId++;
            if (++iter == mDocs.cend()) break;
        }

        for (int i=0; i < indexThread.size(); ++i){
            if (indexThread[i].joinable()) indexThread[i].join();
        }
    }
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

//std::map<std::string, float> mIDF;
void InvertedIndex::CalculateIDF(){
    size_t N = mDocs.size();
    for (const auto &docFreq:mFreqDictionary){
        std::string currentWord = docFreq.first;
        size_t  nqi = docFreq.second.size(); //количество документов в которых есть слово
        double IDF = std::log(1.0 * N / nqi);
//        float IDF = std::log10f((1.0 * N - 1.0*nqi+0.5 )/( 1.0*nqi+0.5));
         mIDF.insert(std::pair(currentWord, IDF));
    }
}

void InvertedIndex::CalcalateAVGDL(){
    mAVGDL = 1.f*mAllDocLengthInWord / mDocs.size();
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

int InvertedIndex::GetWordCountInDoc(const std::string& word, size_t docId){
    for (const auto &entry:GetWordCount(word)){
        if (entry.docId == docId) return entry.count;
    }
    return 0;
}

size_t InvertedIndex::GetDocCount(){
    return mDocs.size();
}

std::string InvertedIndex::GetDoc(size_t docId){
    return mDocs[docId];
}