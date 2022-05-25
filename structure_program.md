# Структура программы
Программа построена из следующих основных классов:

ConverterJSON
-
    ConverterJSON(std::string pathConfigJSON = defaultConfigFileName,
                  std::string pathRequestJSON = defaultRequestsFileName,
                  std::string pathAnswersJSON = defaultAnswerFileName);

Конструктор класса, параметры имена файлов конфигурации, запросов, и файла в который будут сохранены результаты работы поисковой программы. 

    std::vector<std::string> GetRequests();

Метод получения запросов из файла ```requests.json```
возвращает список запросов из файла ```requests.json```

    std::vector<std::string> GetFileNames();

Метода возвращает вектор имён файлов для индексации
возвращает вектор строк имен файлов

	int GetResponsesLimit();

Метод считывает поле mMaxResponses для определения предельного количества ответов на один запрос
возвращает максимальное количество результатов поиска ```max_responses```

    std::string GetNameOfApp();

Метод считывает поле наименования программного продукта
Возвращает строку с наименованием программного продукта

    std::string GetVersionApp();

Метод считывает поле актуальная версия программного продукта, должна совпадать с внутренней версией программы (конфигурация программы должна поддерживаться текущей версией ПО)
Возвращает строку-версия файла ```config.json```

    int GetMethodOfSearch();

Метод возвращает метод поиска установленный в конфигурационном файле. Далее передаётся в метод [Search()](). 

    bool IsValidVersion();

Метод проверяет соответствие версии ПО и версии JSON файла config
Возвращает ```true``` если версия файла ```config.json``` совпадает с версией программы

    void PutAnswers(const std::vector<std::vector<RelativeIndex>> &answers);

Метод вывода результатов поиска в файл ```answers.json```. 
Передаваемый параметр вектор результатов поиска.   

    void SetConfigFileName(const std::string &fileName);
    void SetRequestsFileName(const std::string &fileName);
    void SetAnswerFileName(const std::string &fileName);

Методы изменения дефолтных имён файлов конфигурации, поисковых запросов, и файла вывода результата поиска.
Для всех методов передаваемый параметр имя соответствующего файла. 

 InvertedIndex
-
    void UpdateDocumentBase(const std::vector<std::string> &fileNames);

 Метод для обновления или заполнения, если ранее заполнена не была, базы документов, по которой будем совершаться поиск. Метод однопоточный.
 Входные параметры вектор строк-имён файлов для индексации
 
    void UpdateDocumentBase(const std::vector<std::string> &fileNames, const int &maxThreads);

Метод для обновления или заполнения, если ранее заполнена не была, базы документов, по которой будем совершаться поиск. Метод многопоточный, количество потоков задаётся в параметрах.
 Входные параметры  
 fileNames - вектор строк-имён файлов для индексации;  
 maxThreads - число, количество создаваемых потоков
 
    std::vector<Entry> GetWordCount(const std::string &word);

 Метод определяет количество вхождений слова word в загруженной базе документов. Возвращает подготовленный список с частотой слов.
Входные параметры  word слово, частоту вхождений которого необходимо определить
 
    int GetWordCountInDoc(const std::string &word, size_t docId);

 Метод определяет количество вхождений слова word в документ docId
Входные параметры 
word - слово количество вхождение которого определяем  
docId  - номер документа в котоом определяем количество вхождений слова   Возвращает количество вхождение слова word  в документ docId

    size_t GetWordCountAllDocs(const std::string &word);

Метод определяет количество вхождений слова word в загруженной базе документов во всех имеющихся документах.
Входные параметры
word слово, количество вхождений которого необходимо определить
Возвращает число количество вхождений слова во всех документах

    double GetAVGDL();
 
Метод возвращает среднюю длину документа в списке поиска в словах

    double GetIDF(const std::string &word);

Метод возвращает inverse document frequency IDF, заданного слова
Входные параметры:
word - слово для которого вычислялась IDF  
docId - документ для которого вычисляется IDF  
Возвращает число - IDF
 
    size_t GetDocLength(const size_t &docID);

Метод возвращает длину документа dicId в словах
Входные параметры 
 docID - номер документа длину которого узнаём
Возвращает длину документа в словах
 
    size_t GetDocCount();

Метод возвращает общее количество проиндексированных документов в базе
 
    std::string GetDoc(size_t docId);

Метод возвращает содержание документа docId
Входные параметры 
 docId - номер документа


    void SetMaxWordInDocument(int _MaxWordInDocument);

Метод задания уставки максимального количества обрабатываемых слов в документе. Более данного количества, слова игнорируются. 
 Входные параметры
_MaxWordInDocument - максимальное допустимое количество слов в документе

    int GetMaxWordInDocument() { return mMaxWordInDocument; };

Метод возвращает текущее максимально допустимого количества обрабатываемых слов в документе
 Возвращает установленное максимально допустимое количество слов в документе
 
    void SetMaxCharInWord(int _MaxCharInWord);

Метод задания уставки максимального допустимого  количества символов в слове. Более этого количества, слово будет усекаться.
Входные параметры 
_MaxCharInWord - - максимально допустимое количество символов в слове

    int GetMaxCharInWord() { return mMaxCharInWord; };

Метод возвращает установленное максимально допустимое количество символов в слове

 SearchServer
-

	SearchServer(InvertedIndex &idx, int maxRequests = 5, int methodSearch = 1);

Конструктор.  
idx в конструктор класса передаётся ссылка на класс InvertedIndex, чтобы SearchServer мог узнать частоту слов, встречаемых в запросе и другие параметры индексации документов

    std::vector<RelativeIndex> SearchByQuery_method2(const std::string &queryInput);

Метод поиска документов соответствующих запросу метод подразумевает наличие **всех** слов в документе имеющиеся в запросе. Релевантность вычисляется подсчётом количества слов в документе,сумма количества всех слов запроса, встречающихся в документе затем нормируется.  
В случае отсутствия одного из слов запроса в документе возвращается пустой результат
queryInput - строка запрос  
Возвращает вектор содержащий документID и его релевантность данному запросу
     
     
    std::vector<RelativeIndex> SearchByQuery_method3(const std::string &query);

Метод поиска документов соответствующих запросу. Метод подразумевает наличие хотя бы одного  слова в документе из запроса. Релевантность вычисляется подсчётом количества слов в документе, сумма количества всех слов запроса, встречающихся в документе затем нормируется.
Пустой результат возвращается если нет ни **одного** документа содержащего хотя бы одно слово из запроса.
    
queryInput - строка запрос
Возвращает вектор содержащий документID и его релевантность данному запросу

    std::vector<RelativeIndex> SearchByQuery_BM25(const std::string &query);

Метод поиска документов соответствующих запросу. Метод поиска основан на функции вычисления релевантности Best Match 25.
queryInput - строка запрос
Возвращает вектор содержащий документID и его релевантность данному запросу

    std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::string> &queriesInput);


Метод обработки поисковых запросов. Обрабатывает вектор (несколько поисковых запросов), по методу установленному SetMethodOfSearch. По умолчанию используется метод 1 (BestMatch25). 
queries_input поисковые запросы   
Возвращает отсортированный список релевантных ответов для заданных запросов

    void SetMethodOfSearch(int mMethodOfSearch);


 Метод задания метода поиска. Допустимые входные параметры:  
	 1 - BestMatch25   
     2 - полное совпадение/наличие  
     3 - неполное совпадение/наличие
 

    void SetMaxRequests(int maxRequests);

Метод задания максимального количества выдаваемых результатов по запросу
