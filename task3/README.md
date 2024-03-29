### Алгоритм  
Алгоритмом шифрования в данной задаче является шифр Цезаря в общем виде - когда каждая буква сдвигается на одно и то же число букв в алфавите (считается, что после последней буквы алфавита снова идёт первая). Например, для сдвига 3 в русском алфавите: а $\to$ г, ю $\to$ б.  
И в данной задаче для каждого предложения используется свой размер сдвига.  
Основные действия:  
- Найти "зацепку", по которой можно восстановить ключ шифрования. Почти каждый адрес заканчивается на "кв. N", а почта - на "@<dom>.com" или "@<dom>.org". Проверить, что на приведённых данных гипотеза работает. В данном случае она действительно работает.  
- Для каждого адреса можно найти ключ шифрования (сдвиг), посмотрев на слово перед последним числом - это сокращение от "кв."  
- Для каждого адреса электронной почты "зацепка" - последние три буквы: они являются сдвигом либо "com", либо "org".  
- Найдя сдвиг, можем легко дешифровать адрес/почту (не забыв в конце привести буквы к исходным регистрам). При дешифровании меняем только буквы (русского или английского языка) - цифры, пунктуацию и прочее оставляем без изменений.  

### Что где смотреть в репозитории (ссылка ниже):  
- Алгоритм с дополнительными пояснениями представлен в файле 'PetropolitanReSearch3.ipynb'.  
- Файл с результатом работы называется 'task3_results.csv'.  
- Блок-схему алгоритма можно посмотреть в файле 'Fowchart.pdf'  
Примечание: в блок-схеме под "циклический сдвигом" подразумевается сдвиг каждой буквы на одно и то же число позиций по зацикленному (после последней букв снова первая) алфавиту.  
