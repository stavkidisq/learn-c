### Quastions & Answers на собеседовании.
#### Что такое NULL указатель?
*Нулевой указатель (null pointer)* - это указатель,который указывает ни на какой объект. Если нам не нужно, чтобы указатель хранил ссылку на какой-то объект, 
то мы присваиваем ему специальное значение (null value). Для этого мы можем использовать следующий код
```c
int* first_pointer = nullptr;
int* second_pointer = NULL;
int* third_pointer = 0;

printf("nullptr is: %p\n", first_pointer); // Выведет: nullptr is: 0000000000000000
printf("NULL is: %p\n", second_pointer); // Выведет: NULL is: 0000000000000000
printf("0 is: %p\n", third_pointer); // Выведет: 0 is: 0000000000000000
```
