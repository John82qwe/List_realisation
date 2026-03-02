#include <stdio.h>
#include <stdlib.h>

#define foreach(__iter, __main) for((__iter) = (__main); (__iter) != 0; __iter = __iter->next)
#define ERROR(condition, rtr)\
do {\
	if (condition) {\
		printf("Ooopss. Something goes wrong!"); \
		return rtr; \
	}\
}while(0)

#define ERROR_Void(condition)\
do {\
	if (condition) {\
		printf("Ooopss. Something goes wrong!"); \
		return; \
	}\
}while(0)


typedef struct node_of_list {
	struct node_of_list* next;
	int element;    //  потом T 
}node_of_list;

typedef struct list {
	struct node_of_list* head;
	size_t size;
}list;

node_of_list* get_node(list* ls, size_t index) { // получение node по ее индексу! // передать list
	if ((ls != NULL) && (index < ls->size)) { // size
		node_of_list* result_node = ls->head;
		for (size_t i = 0; i < index; i++) {
			result_node = result_node->next;
		}
		return result_node;

	}
	ERROR((index >= ls->size) || (ls == NULL), NULL);
}

void create_nodes(list* ls, size_t prev_index, int* elements, size_t size) {  // предаем массив элементов и на основе его создаются новые node(друг за другом)
	if (prev_index < ls->size && elements != NULL && ls != NULL) {
		node_of_list* prev = get_node(ls, prev_index);
		if (prev != NULL) {
			for (size_t i = 0; i < size; i++) {
				node_of_list* new_node = malloc(sizeof(node_of_list));
				if(new_node != NULL) {
					new_node->element = elements[i];
					new_node->next = prev->next; 
					prev->next = new_node;
					prev = new_node;
					ls->size += 1;
				}
				ERROR_Void(new_node == NULL);

			}
		}
		ERROR_Void(prev == NULL);
	}
	ERROR_Void(prev_index >= ls->size || elements == NULL || ls == NULL); // переделать
}

list* create_list(node_of_list* head) {
	list* ls = malloc(sizeof(list));
	if (ls != NULL) {
		if (head != NULL) {
			size_t size_of_list = 1;
			ls->size = size_of_list;
			ls->head = head;
			return ls;
		}
		else {
			size_t size_of_list = 0;
			ls->size = size_of_list;
			ls->head = head;
			return ls;
		}
	}
	ERROR(ls == NULL, NULL);
	
}

node_of_list* create_head(list* list_of_nodes, int element) {
	if (list_of_nodes->head == NULL && list_of_nodes->size == 0) {
		node_of_list* new_node = malloc(sizeof(node_of_list));
		if(new_node != NULL) {
			new_node->element = element;
			new_node->next = NULL;
			list_of_nodes->size += 1;
			list_of_nodes->head = new_node;
			return new_node;
		}
		ERROR(new_node == NULL, NULL);
	}
	ERROR(list_of_nodes->head != NULL || list_of_nodes->size != 0, NULL);
}



size_t get_node_count(list* ls) {
	if (ls != NULL) {
		return ls->size;
	}
	ERROR(ls == NULL, 0);
}

node_of_list* create_node(list* ls, size_t prev_index, int element) {  // создаем один node
	if (prev_index < ls->size && ls->head != NULL && ls != NULL) {
		node_of_list* prev = get_node(ls, prev_index);
		if (prev != NULL) {
			node_of_list* new_node = malloc(sizeof(node_of_list));
			if (new_node != NULL) {
				new_node->element = element;
				new_node->next = prev->next;
				prev->next = new_node;
				ls->size += 1;
				return new_node;
			}
			ERROR(new_node == NULL, NULL);
		}
		ERROR(prev == NULL, NULL);

	}
	else if (ls->head == NULL && prev_index == 0){
		ls->head = create_head(ls, element);
		return ls->head;
	}
	ERROR(prev_index >= ls->size || ls == NULL, NULL);
}




size_t get_node_index(list* ls, node_of_list* node) { 
	if (node != NULL && ls != NULL) {
		node_of_list* i;
		size_t index_node = 0;
		foreach(i, ls->head) {
			if (i == node) {
				return index_node;
			}
			index_node++;
		}
	}
	ERROR(node == NULL || ls == NULL, 0);
}

// функция добавления node в список
void add_node_to_list(list* ls, size_t prev_index, int elemnt_of_node) { // prev_index - node, после которого нужно вставить элемент
	if (ls != NULL && prev_index < ls->size) {
		node_of_list* prev = get_node(ls, prev_index);
		node_of_list* next_node = prev->next; // позиция в list: .... prev, next_node....
		node_of_list* new_node = malloc(sizeof(node_of_list));
		if(new_node != NULL) {
			new_node->element = elemnt_of_node;
			new_node->next = next_node;
			prev->next = new_node; // позиция в list: .... prev, new_node, next_node....
			ls->size += 1;
		}
		ERROR_Void(new_node == NULL);
		
	}
	ERROR_Void(prev_index >= ls->size || ls == NULL);
}

void remove_node_from_list(list* ls, size_t index_of_node_to_remove) { 
	if (index_of_node_to_remove < ls->size && ls != NULL) {
		node_of_list* node_to_remove = get_node(ls, index_of_node_to_remove);
		if (node_to_remove != ls->head) {
			node_of_list* node;
			for (node = ls->head; node->next != node_to_remove; node = node->next) {} 
			node->next = node_to_remove->next;
			free(node_to_remove);
			ls->size -= 1;
		}
		else {
			ls->head = node_to_remove->next;
			free(node_to_remove);
			ls->size -= 1;
		}
	}
	ERROR_Void(index_of_node_to_remove >= ls->size || ls == NULL);
}

void print_list(list* ls) { // функция для вывода всех существующих node
	if (ls != NULL) {
		node_of_list* node;
		size_t index = 0;
		printf("___________________list of %zu elements:___________________\n", ls->size);
		foreach(node, ls->head) {
			printf("num: %zu |next node address: %p | element: %d\n|\nV\n", index, node->next, node->element);
			index += 1;
		}// __iter переменная в которую извлекается узел
		
	}
	ERROR_Void(ls == NULL);
	
}

// функция удаления list из памяти

void delete_list_from_memory(list* ls) { // удаление list из памяти (всех узлов)
	if (ls != NULL) {
		node_of_list* current_node = ls->head;
		
		while(current_node != 0){
			node_of_list* new_node = current_node->next;
			free(current_node);
			current_node = new_node;
		}
		free(ls);
		printf("List was successfully deleted!\n");
		
	}
	ERROR_Void(ls == NULL);
}


int main() {
	int elements_of_nodees[] = { 1, 2, 3, 4 };
	list* ls = create_list(NULL); // создаем пустой list
	create_head(ls, 11); // создаем голову списка в пустом массиве
	create_nodes(ls, 0, elements_of_nodees, 4); 
	
	create_node(ls, get_node_count(ls)-1, 666); 
	add_node_to_list(ls, 2, 999);
	
	node_of_list* third_node = get_node(ls, 0);
	node_of_list* first_node = get_node(ls, 1);
	printf("___________%zu__________\n", get_node_index(ls, first_node));
	remove_node_from_list(ls, 0);
	print_list(ls);
	printf("%zu ", get_node_count(ls));
	delete_list_from_memory(ls); 
	ls = NULL; 
	printf("_____________________________________________________");
	print_list(ls); 
	return 0;
}