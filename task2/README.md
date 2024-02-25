## Алгоритм  
Рассмторим все углы от -90 до 90 градусов и выберем среди них наиболее подходящий.  
Для каждого угла-кандидата:  
1. Конвертируем картинку в 2D посредством библиотеки pillow.  
2. Применим к исходному изображению поворот на рассматриваемый угол - результат запишем в новую переменную.  
3. Посчитаем сумму значений пикселей по строкам. Посчитаем дисперсию выборким значений построчных сумм. Если картика повёрнута правильно, то строки будут то захватывать текст, то только фон, и из-за этого дисперсия будет большой.  
4. Значение угла, для которого дисперсия максимальна - и есть ответ, который надо вернуть.  

## Использование ресурсов  
- Из дополнительной памяти нам нужна дополнительная картинка - куда мы будем сохранять результат поворота при подборе нужного угла. Стоит отметить, что результаты поворота можно записывать в одну и ту же переменную, так что памяти это потребует довольно немного. Также нужен массив из 180 float-ов - туда будем записывать значения дисперсии для каждого из углов.  
- По времени самая затратная операция - это поворот. Она подразумевает перемещение всех пикселей и выполняется 180 раз (для каждого угла от -90 до 90). На практике это довольно быстро.  
- Алгоритм не подразумевает итераций по самой картинке, которые обычно очень долгие. Просто поворот, а потом - использование *numpy*.
- Алгоритм не отличает углы поворота с разницей 180 градусов, но это и не требуется, так как подразумевается, что модуль угла поворота не больше 90 градусов (видимо, от -90 до 90 не включтельно). Это разумное ограничение, так как при фотографировании текста если и получается криво, то не вверх ногами.  

## Что где  
1. В файле *task2.py* приведён код алгоритма.  
    - Функция *find_angle* находит угол, на который было повёрнуто изображение, а *unrotate* поворачивает изображение назад (и возвращает угол поворота).  
    - Как запустить: в данной директории (*task2*) выполнить команду `python3 ./task2.py "rotated_picture.jpg" "save_result_here.jpg"`, где первый путь - до повёрнутого изображения, а второй указывает, куда сохранить результат (выровненное изображение).  
    - В *src* можно найти примеры картинок.  
    - В *benchmarking* лежит *ipython notebook* с экспериментами, а также 6 картинок, нужных для эксперимента. Алгоритм отлично справился с определением угла поворота на всех картинках (подробнее - см. *benchmarking/task2_tests.ipynb*)  
