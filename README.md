// данные программы были написаны в интегрированной среде разработки CoIDE.
// Код был отлажен на стенде Pinboard II. 
//стенд построен на базе микроконтроллера ARM Cortex-M3 STM32F103x8
// файл indicator выполняет следующую программу:

“бегущий огонек”, занимающий одну позицию
на линейке светодиодов, продвигался сверху вниз, каждый раз перескакивая через одну позицию. 
Например, сначала *ххххххх, потом хх*ххххх, потом хххх*ххх и т. д.
По достижении нижнего края индикатора процесс должен циклически повторяться. * — светодиод светится,
х — светодиод не светится.

// файл "semisegment indicator" выполняет следующую программу:

Выводить в двух правых разрядах семисегментного индикатор число, начиная с 1, 
увеличивающееся каждый раз в два раза. Каждое состояние должно индицироваться в течение 1 с.

// файл "vnesh prerivania" выполняет следующие программы:

Основная программа. Последовательно должны светиться три следующие комбинации
****хххх, хх****хх, хххх****. Затем процесс должен циклически повторяться.
Подпрограмма обработки прерывания. Выводить убывающие числа от FFh до 0 в двух разрядах
семисегментного индикатора (левом и среднем), оставшийся разряд должен быть погашен. 
Каждое число до изменения должно индицироваться в течение 1 с.

//файл "ADC programming" выполняет следующие программы:
Запрограммировать на клавиатуре кнопки таким образом, чтобы при нажатии одной из них движение
бегущего огонька на линейке светодиодов было слева направо,при нажатии другой — справа налево;
нажатия на третью приводили бы к увеличению скорости движения огонька (чем больше нажатий,
тем больше скорость); нажатия на четвертуюприводили бы к уменьшению скорости движения
огонька (чем больше нажатий, тем меньше скорость).При этом скорость движения огонька
(в условных единицах) должна отображаться на семисегментном индикаторе.

