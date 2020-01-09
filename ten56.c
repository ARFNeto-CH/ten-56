#define	_CRT_SECURE_NO_WARNINGS

#include "malloc.h"
#include "memory.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct exemplo
{
	int		i_parametro;
	double	d_parametro;
	char*   s_parametro;
	int		(*f_parametro_i)(int);
	void	(*f_parametro_v)(int, int);
};
typedef struct exemplo Exemplo;

Exemplo*    aloca_exemplo(int);
int*        aloca_exemplo_range(char*);
int			calcula_offset(const int[], const int[], const int);

void        listoff(const int[], const int[], int);
void        listoff_str(const char*, const char*);	// mais pratica pra testar

//
// https://www.clubedohardware.com.br/forums/topic/1419843-exercicio-sobre-vetores-multidimensionais/
//
int main(int argc, char** argv)
{
	int         n = 125;

	Exemplo*    p = aloca_exemplo(n);
	if (p == NULL) exit(-1);
	printf("so pra testar, ultimo valor dos %d a partir de 0 = %d\n", n, (*(p+n-1)).i_parametro);
	free(p);

	int* ex_range = aloca_exemplo_range("3 5 10 6 3");

	const int range[2] = { 24,80 };
	const int index[2] = { 23,79 };
	int offset = calcula_offset(range, index, 2);
	printf("offset para a[24][80] em int a[23,79] = %d\n", offset);

	const int range5[5] = { 3,5,10,6,3 };
	const int index5[5] = { 2,4,9,5,2 }; 
	offset = calcula_offset(range5, index5, 5);
	printf("offset para o ultimo elemento de a[3][5][10][6][3] = %d\n", offset);

	const int index6[5] = { 1,3,1,2,0 };
	offset = calcula_offset( range5, index6, 5);
	printf("offset para o a[1][3][1][2][0] em int a[3][5][10][6][3] = %d\n", offset);

	listoff_str("3 5 10 6 3", "1 3 1 2 3");
	listoff_str("3 5", "4 4");
	listoff_str("3 5 2", "2 6 2");

	return 0;
}	// main()


Exemplo* aloca_exemplo(int total)
{
	int			a_alocar = total * sizeof(Exemplo);
	Exemplo* pE = (Exemplo*)malloc(a_alocar);
	if (pE == NULL) return pE;
	for (int i = 0; i < total; i += 1)(*(pE + i)).i_parametro = i;
	return pE;
};


//
// aloca um vetor como no livro
//
int* aloca_exemplo_range(char* range)
{
	char Range[81];
	strncpy(Range, range, 80);
	unsigned int sz = 1;
	int planos = 0;
	char* valor;
	const char* delim = " ";
	printf("range = [%s]\n", range);
	valor = strtok(Range, delim);
	while (valor != NULL)
	{
		sz = sz * atoi(valor);
		planos = planos + 1;
		valor = strtok(NULL, delim);
	};
	printf("\nrange is = [%s]\n\tplanes = %d\n\tunit size = %d\n\
tquant = %d\n\ttotal size = %d\n\n",
		range, planos, sizeof(int), sz, sz*sizeof(int) );

	return (int*) malloc( sz*sizeof(int));
};


int calcula_offset(const int range[], const int index[], const int size)
{
	int offset = 0;
	for (int j = 0; j < size; j++)
		offset = range[j] * offset + index[j];
	return offset;
};	// calcula_offset()


void listoff(const int* range, const  int* sub, int n)
{
	// addr = base(ar) + esize*offset; by the book
	printf("\nn = %d\n",n);
	printf("Range: ");
	for (int i = 0; i < n; i++) printf("%d ", range[i]);
	printf("\n");
	printf("  Sub: ");
	for (int i = 0; i < n; i++) printf("%d ", sub[i]);
	printf("\n");
	//
	int esize = 1;
	int last_plane = -1;

	for (int i = 0; i < n; i++)
	{
		if (sub[i] >= range[i])
		{
			last_plane = i;
			break;
		}
	}	// for

	if (last_plane == -1)
	{
		printf("unico elemento: addr = base + 1 * (%d)\n", calcula_offset(range, sub, n));
		return;
	};	// if

	// vai listar um monte de gente talvez...
	int* novo = (int*) malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) novo[i] = sub[i];
	for (int i = last_plane; i < n; i++)
	{
		novo[i] = 0;
	};
	printf("\nBase: ");
	for (int i = 0; i < n; i++) printf("%d ", novo[i]);
	printf("\n");

	printf("offset da base : %d ", calcula_offset(range, novo, n));

	// final
	for (int i = last_plane; i < n; i++)
	{
		novo[i] = range[i] - 1;
	};	// for
	printf("offset do final: %d ", calcula_offset(range, novo, n));
	free(novo);
	return;
};	// listoff()


void        listoff_str(const char* range_str, const char* sub_str)
{
	// cria os vetores de int on-the-fly para facilar os testes de listoff()
	// usando as strings de entrada
	printf("\n\nteste listoff(\"%s\",\"%s\",n) ... listoff_str(char* range, char* sub)\n", range_str, sub_str);

	char vetor[81];
	strncpy(vetor, range_str, 80);
	char* valor;
	const char* delim = " ";
	valor = strtok(vetor, delim);
	int planos = 0;
	while (valor != NULL)
	{
		planos = planos + 1;
		valor = strtok(NULL, delim);
	};
	printf("\n\tdimensoes = %d\n", planos);
	// agora cria o vetor e converte
	strncpy(vetor, range_str, 80);
	int* range = malloc(planos * sizeof(int));
	valor = strtok(vetor, delim);
	int plano = 0;
	while (valor != NULL)
	{
		range[plano] = atoi(valor);
		plano += 1;
		valor = strtok(NULL, delim);
	};
	// agora mostra o vetor criado
	printf("\tVetor range: ");
	for (int i = 0; i < planos; i++)
		printf(" %d", range[i]);
	printf("\n");

	// agora o sub

	strncpy(vetor, sub_str, 80);
	int* sub = malloc(planos * sizeof(int));
	valor = strtok(vetor, delim);
	plano = 0;
	while (valor != NULL)
	{
		sub[plano] = atoi(valor);
		plano += 1;
		valor = strtok(NULL, delim);
	};
	// agora mostra o vetor criado
	printf("\tVetor   sub: ");
	for (int i = 0; i < planos; i++)
		printf(" %d", sub[i]);
	printf("\n");

	printf("\nChamando listoff com os vetores criados. de ordem %d\n", planos);
	listoff(range, sub, planos);

	free(range);
	free(sub);
	return;
};	// listoff_str()
