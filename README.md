# Реализация системы поиска   
Приложение поиска документов по нескольким запросам.  
[![](/Pictures/pictogram.jpg)]()

## СОДЕРЖАНИЕ:
* [Постановка задачи](#Постановка-задачи)
* [Использованные библиотеки](#Использованные-библиотеки)
* [Реализованные методы поиска](#Методы-поиска)
* [Структура программы](#Структура-программы)
* [Сборка приложения](#Сборка)
* [Конфигурирование приложения](#Конфигурирование)
* [Запуск приложения](#Запуск)
* [Результат работы приложения](#Результат)
* [Планируемые улучшения](#Планы-улучшщения)

### <a name="Постановка-задачи">Постановка задачи</a>
Необходимо разработать поисковый движок который можно использовать для поиска по своему набору документов. Алгоритм и принцип работы при необходимости можно менять и развивать. Поисковый движок должен представлять консольное приложение, осуществляющий поиск и имеющее возможность настройки через файлы формата [JSON](https://www.json.org/json-ru.html). 

### <a name="Использованные-библиотеки">Использованные библиотеки</a>
При разработке данной программы были использованы следующие библиотеки
- [JSON for MODERN C++](https://github.com/nlohmann/json)
- [A C++17 Thread Pool for High-Performance Scientific Computing](https://github.com/bshoshany/thread-pool)
- [GoogleTest](https://github.com/google/googletest)


### <a name="Методы-поиска">Реализованные методы поиска</a>
Реализовано 3 метода поиска запроса в документах:
- 1 - метод [Best Match 25](https://en.wikipedia.org/wiki/Okapi_BM25)
- 2 - метод полного соответствия запросу (в документе должны присутствовать все слова запроса)
- 3 - метод частичного соответствия запросу (в документе должны присутствовать от одного слова запроса)

**Метод 1:** Рассчитывается score для каждого документа, затем результат нормируется;  
  
**Метод 2:** Находится наименее встречаемое слово из запроса, затем производится поиск остальных слов в найденных документах,
затем вычисляется суммарное вхождения слов запроса, производится нормировка и выводится результат.
Если нет одного из слов в документе, то документ из списка изымается, таким образом, для того чтобы документ попал в результаты поиска
необходимо наличие в документе всех слов запроса.  
   
**Метод 3:** Производится расчёт количества вхождения каждого слова запроса в документе, затем вычисляется суммарное вхождение всех слов запроса в документ, результат нормируется и выводится в файл. Документ попадёт в результаты поиска, если хотя бы одно из слов запроса присутствует в документе.
 
### <a name="Структура-программы">Структура программы</a>
Программа состоит из следующих базовых классов:

- ConverterJSON
- InvertedIndex
- SearchServer

Более детально со структурой программы и кратким описанием классов и методов можно ознакомиться [здесь](https://github.com/DikoSt/SearchEngine/blob/main/structure_program.md)

### <a name="Сборка">Сборка приложения</a>
Сборка и проверка функционирования приложения производилась под управлением операционной системе Windows7, Wndows10.
Проверена сборка на следующих:  

* Microsoft Visual Studion 16 2019  
* minGW64 v6.0

		mkdir build
		cd build
		cmake ..
		cmake --build . --config Release --target SearchEngine

Для сборки утилит создания стартовых json файлов

		cmake --build . --config Release --target createJSONs

Запустить createJSONs.exe без параметров файлы будут созданы в папке запуска файла:

		cd ..
		cd Utilites/Release
		createJSONs.exe
В папке Utilites/Release будут созданы файлы config.json и requests.json
Для работы приложения необходимо эти файлы переместить в папку приложения

		copy *.json ..\\..\\Release
		
Сконфигурировать приложение и оно готово к работе
		
		cd ..\\..Release
		SerchEngine.exe

Для демонстрации работы поисковой системы предлагается несколько тестовых файлов. Для демонстрационного запуска необходимо выполнить следующее:
убедиться что находимся в папке, в которой находится searchengine.exe (В данном примере папка Release):

		mkdir resource
		copy ..\\resource\*.txt resource
		copy ..\\resource\*.json .\ 

Для запуска Unit-testов встроенных необходимо в папке build, ранее созданной выполнить следующее:

	cmake --build . --config Release --target Google_Tests_All_Test
	cd ..\\Release
	Google_Tests_All_Test.exe
		
### <a name="Конфигурирование">Конфигурирование приложения</a>
Основные настройки/конфигурация программы содержится в файле config.json.
Файл должен располагаться в корневой директории приложения.

Пример файла конфигурации приведён ниже:

    {
        "config": {
            "max_responses": 5,
            "method_search": 1,
            "name": "SearchEngine",
            "version": "0.1"
        },
        "files": [
            "resource\\file0.txt",
            "resource\\file1.txt",
            "resource\\file2.txt"
        ]
    }

, где 
**max_responses** - количество отображаемых ответов в файле answers.json  
**method_search** - [используемый метод поиска](#Методы-поиска)  
**name** - отображаемое имя приложения при запуске.  
**version** - версия приложения/JSON конфигурационного файла, должна совпадать с текущей версией поисковой программы.  
**files:** - перечень файлов среди которых производиться поиск   
   "resource\\file0.txt" - относительный путь к файлу. Допускается использовать полный путь файлам:  
   "C:/libraryOfFile/file.txt"

Файлы для поиска должны быть текстовыми, содержать латинские символы. Спец символы, знаки препинания и т.п. при индексировании и поиске игнорируются 
Количество файлов ограничено здравым смыслом :)

Для задания запросов поиска необходимо сформировать файл requests.json, Формат и пример файла requests.json приведён ниже.
        
        {
        "requests":[
            "milk water", 
            "sugar", 
            "milk water sugar"
            ]
        }
     
, где  
**requests** - индетификатор запросов (обязательное поле);  
"***milk water***", "***sugar***", "***milk water sugar***" - поисковые запросы. 

К файлу *requests.json* и поисковым запросам предъявляются следующие требования/ограничения:  
- в файле может быть не более 1000 запросов(остальные будут игнорироваться);
- запрос выполняется прописными латинскими буквами;
- в запросе может быть не более 10 слов ("лишние" слова будут игнорироваться);
- в слове запроса допускается не более 100 символов (?)

### <a name="Запуск">Запуск приложения</a>
После подготовки файла конфигурации и файла поисковых запросов, приложение готово к запуску.
Для запуска приложения необходимо выполнить следующие условия:
- Файлы config.json и request.json должны находиться в одной папке с файлом SearchEngine.exe;  
- Относительные пути к файлам-документам должны строиться от папки в которой запускается SearchEngine.exe;

Запуск приложения выполняется запуском файла SearchEngine.exe на исполнение. После запуска приложения в консоли будет отображена следующая информация:

    Start SearchEngine
    Version: 0.1
    Max request limit: 4
    Method search: 2

В случае возникновения нештатных ситуаций будет выведены дополнительные сообщения, и возможно досрочно прекращена выполнение программы.
Порсле удачного завершения работы программы  "Process finished with exit code 0", в директории запуска программы будет создан файл answers.json.
Формат результирующего файла описан [ниже](#Результат). 

### <a name="Результат">Результат работы приложения</a>
Результатом работы приложения является созданный файл answer.json
Пример файла ниже:

    {
        "answers": {
            "request001": {
                "result": false
            },
            "request002": {
                "relevance": [
                    {
                        "docID": 4,
                        "rank": 1.0
                    }
                ],
                "result": true
            }
     }

* **answers** — базовое поле в этом файле, которое содержит ответы на запросы.
* **request001 … 002** — идентификатор запроса, по которому сформирован ответ.
* **result** – результат поиска запроса. Если он принимает значение ***true***, значит по
  данному запросу найден хотя бы один документ. Если результат имеет значение
  ***false***, значит ни одного документа не найдено.
* **relevance** включается в файл ***answers.json***, если на этот запрос удалось найти
  более одного документа.
  * <Идентификатор документа>("docID") — идентификатор документа, в котором найден ответ на запрос. Например, если в поле config.json поле files содержит:                       
    
			"files": [
 				"file001.txt", для данного файла docid будет равен 0
 				"file002.txt"  для данного файла docid будет равен 1
			]
  * **<ранг ответа>(“rank”)** — ранг или поисковый рейтинг. Это число показывает,
    насколько документ подходит для заданного запроса. В ответе id документов
    располагаются в порядке уменьшения поискового рейтинга.
 
  
## <a name="Планы-улучшщения">Планируемые улучшения</a>

- [х] Оптимизация метода поиска №2 
- [х] Оптимизировать/реализовать многопоточную индексацию
- [х] Реализовать хранение только индексов 
- [ ] Реализовать метод хранения индексов в файле
- [ ] Метод определения необходимости запуска индексации
- [ ] Разработка конфигуратора поисковой системы с GU-интерфейсом
