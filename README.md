# ***STEGANOGRAPHY***
`src` folder contains the source code of the program:
* `main.c` - processes command line arguments and calls respective functions for different modes
* `bmp_steg.[ch]` - provides functions for encoding messages into BMP files and decoding messages from BMP files
* `errors.h` - defines user errors
* `help.c` - contains help menu, generated with `xxd -i help.txt > src/help.c` command

# Криптиране и декриптиране на информация във визуални файлове
Да се състави програмен продукт, който да използва конзолен интерфейс за 
комуникация с потребителя.
## Цел
Цел на програмата е да демонстрира възможността за „скриване“ на определено количество данни в изображения, минимално модифицирайки цветовите данни на входящия файл. Този метод на скриване на информация се нарича [стеганография](https://en.wikipedia.org/wiki/Steganography) (буквално се превежда скрито писане).
## Функционалност
При стартиране на програмата тя извежда информация за поддържаните от нея функционалности.

Програмата да приема аргументи от командния ред, позволяващи избор на отделните подфункционалности.

опция |  | тип | пример
--- |--- |--- |--- |
-o | Пълната пътека до изходната папка | Текстов низ <br> Подава се в двойни кавички. Ако не се попълни за целева папка се приема текущата. | `-o "C:\Test folder\TestFileDir"`
-i | Пълната пътека до входния файл | Текстов низ <br> Ако не се попълни, програмата приключва с грешка | `-i "C:\Test folder\TestFileDir.bmp"`
-e | Избира действието “encode” | Текстов низ <br> Съобщението, което ще се криптира. При избор на тази опция, е задължително въвеждането на низ с повече от 1 символ. | `-e "Lorem Ipsum"`
-d | Избира действието “decode” | Текстов низ <br> Име на изходен файл. Неговият тип е „.txt” | `-d "SecretMessage"`

## Възможни действия
### *Encode*
За изпълнение на тази команда е нужно да изпълните следните 3 действия:

Първо трябва да прочете входния файл (изберете най-удобния за вас формат ([.bmp](https://en.wikipedia.org/wiki/BMP_file_format), [.jpeg](https://en.wikipedia.org/wiki/JPEG) ,[.png](https://en.wikipedia.org/wiki/PNG) или който и да е) ). Ще откриете, че различните формати имат различни шаблони за представяне на цветовите данни. Проучете как точно изглежда “header” секцията за избрания от вас формат и я изчетете в подходяща структура данни. Избирайте правилно, понеже някои формати имат подводни камъни, които могат доста да усложнят работата ви с тях

Второ – преминете към кодиране на дадената информация по метода в LSB (най-младшият бит на цветовите данни). Това е предпочитаният от нас метод понеже е най-прост за имплементация и дава най-малка визуална разлика (защо?). Ако желаете може да опитате и с различен, стига да може да обясните действието му. При енкодването на информацията помислете как декодиращата част на програмата ще различи непроменените от нас цветови данни от кодираната информация (маркер за начало и край).

Накрая запазете резултатния файл спрямо зададените параметри от потребителя със същото име и формат в зададената директория. Уверете се, че променените от вас изображения продължават да се отварят като такива.

### *Decode*
При използване на тази команда, целта е да извършите обратното действие на “encode”.  Отново прочетете файла (този път този, съдържащ промените). Извадете кодираните данни и запазете резултата спрямо желанието на потребителя.

## Участие
Георги Георгиев - автор на идеята, оценяващ

Йоан Джелекарски - студент, реализиращ проекта

