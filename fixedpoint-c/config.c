#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <errno.h>

// default '0' for less noise
//#define DEBUG 1
#ifdef DEBUG
#define debugf printf
#else
#define debugf(...)
#endif

// parse config file
static int config (const char *name, int *P, int *N, int *A, int *M, int *T, double **F);
static int model_parse (const char *name, int *N, int *M, double **model);

// single processor, non-MPI version for testing
int main(int argc, char** argv)
{
	int rc;

	int P = 0; // number of processors
	int N = 0; // number of instances
	int A = 0; // number of features
	int M = 0; // number of iterations
	int T = 0; // number of top features
	double *F = NULL; // features matrix (N x A+1), contiguous memory

	// argument check
	if (argc != 2) {
		printf("invalid number of args, must be 'mpirun [-np NO_OF_CPUS] ./cmpe300_mpi_2016800027 FILE '\n");
		return -1;
	}
	// parse config file
	rc = model_parse(argv[1], &N, &M, &F);
	if (rc != 0) {
		printf("config parse failed!\n");
		return -1;
	}
	debugf("P(%d) N(%d) A(%d) M(%d) T(%d)\n", P, N, A, M, T);


	free(F);
	return 0;
}

int glue_strncmp (const char *s1, const char *s2, size_t n)
{
	if(!s1 && !s2) return 0;
	if(!s1) return -1;
	if(!s2) return 1;
	return strncmp(s1, s2, n);
}


// parse string and convert to int
static int glue_strtoi (const char *nptr, int *data)
{
	int i;
	char *endptr;
	if (!nptr) {
		printf("null string (int)\n");
		return -1;
	}
	i = (int) strtol(nptr, &endptr, 10);
	if (errno == ERANGE){
		perror("convert (int)");
		return -1;
	}
	*data = i;
	return 0;
}

// parse string and convert to double
static int glue_strtod (const char *nptr, double *data)
{
	double d;
	char *endptr;
	if (!nptr) {
		printf("null string (double)\n");
		return -1;
	}
	d = strtod(nptr, &endptr);
	if (errno == ERANGE){
		perror("convert (double)");
		return -1;
	}
	*data = d;
	return 0;
}

// parse model file
static int model_parse (const char *name, int *N, int *M, double **model)
{
	int i = 0;
	int j = 0;
	size_t len = 0;
	ssize_t read = 0;
	FILE *fp = NULL;

	char * line = NULL;
	char *token = NULL;
	char *saveptr = NULL;
	char delimit[]=" \t\r\n\v\f";

	int _N = 0;
	int _M = 0;
	double *_model = NULL;

	if (!name) {
		printf("null string (file)\n");
		goto err0;
	}
	fp = fopen(name, "r");
	if (!fp) {
		perror("open");
		goto err0;
	}
	/* first line: type of parameters ('M' - model or 'T' - test) */
	if ((read = getline(&line, &len, fp)) == -1) {
		perror("file type");
		goto err1;
	}
	//debugf("%s", line);
	for (token = strtok_r(line, delimit, &saveptr); token != NULL; token = strtok_r(NULL, delimit, &saveptr)) {
		if (glue_strncmp(token, "M", 1) != 0) {
			printf("file type '%s' is not model\n", token);
			goto err1;
		}
		break;
	}
	/* second line:
	    N is the number of rows
	    M is the number of columns */
	if ((read = getline(&line, &len, fp)) == -1) {
		perror("model params");
		goto err1;
	}
	//debugf("%s", line);
	i = 0;
	for (token = strtok_r(line, delimit, &saveptr); token != NULL; token = strtok_r(NULL, delimit, &saveptr)) {
		switch (i) {
			case 0: if (glue_strtoi(token, &_N) != 0) { goto err1; } break;
			case 1: if (glue_strtoi(token, &_M) != 0) { goto err1; } break;
			default:
				printf("invalid N/M line\n");
				goto err1;
		}
		i++;
	}
	// allocate contiguous memory: N x M
	_model = (double *) malloc (_N * _M * sizeof(double));
	if (!_model) {
		printf("model alloc error\n");
		goto err1;
	}

	/* remaining lines: N rows of model data */
	i = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		//debugf("%s", line);
		// instance sanity check
		if (i >= _N) {
			printf("exceeded total instance count (N):(%d)!\n", _N);
			goto err2;
		}
		// parse M columns of model data
		j = 0;
		for (token = strtok_r(line, delimit, &saveptr); token != NULL; token = strtok_r(NULL, delimit, &saveptr)) {
			if (j >= _M) {
				printf("exceeded total item count M:(%d) per instance!\n", _M);
				goto err2;
			}
			// part of the feature vector, parse float
			if (glue_strtod(token, &_model[(i * _M) + j]) != 0) {
				printf("unable to convert '%s' to float @ instance(%d),item(%d)\n", token, i, j);
				goto err2;
			}
			j++;
		}
		// model sanity check
		if (j != _M) {
			printf("not enough columns _M(%d), expected(%d) @ instance(%d)!\n", j, _M, i);
			goto err2;
		}
		i++;
	}
	// instances sanity check
	if (i != _N) {
		printf("not enough rows(%d), expected(%d)!\n", i, _N);
		goto err2;
	}

	if (line)
		free(line);
	fclose(fp);

	// fill pointers
	*N = _N;
	*M = _M;
	*model = _model;
	return 0;

	// graceful exit
err2:	free(model);
err1:	fclose(fp);
		if (line)
			free(line);
err0:	return -1;
}







// parse config file
static int config (const char *name, int *P, int *N, int *A, int *M, int *T, double **F)
{
	int i = 0;
	int j = 0;
	double c = 0;
	size_t len = 0;
	ssize_t read = 0;
	FILE *fp = NULL;

	char * line = NULL;
	char *token = NULL;
	char *saveptr = NULL;
	char delimit[]=" \t\r\n\v\f";

	int _P = 0;
	int _N = 0;
	int _A = 0;
	int _M = 0;
	int _T = 0;
	double *features = NULL;

	if (!name) {
		printf("null string (file)\n");
		goto err0;
	}
	fp = fopen(name, "r");
	if (!fp) {
		perror("open");
		goto err0;
	}
	/* first line: type of parameters ('M' - model or 'T' - test) */
	if ((read = getline(&line, &len, fp)) == -1) {
		perror("file type");
		goto err1;
	}
	//debugf("%s", line);
	for (token = strtok_r(line, delimit, &saveptr); token != NULL; token = strtok_r(NULL, delimit, &saveptr)) {

		break;
	}
	/* second line:
	    N is the number of instances
	    A is the number of features [start from 0 and all numeric with decimal places up to 4]
	    M is the iteration count for weight updates
	    T is the resulting number of features */
	if ((read = getline(&line, &len, fp)) == -1) {
		perror("inputs");
		goto err1;
	}
	//debugf("%s", line);
	i = 0;
	for (token = strtok_r(line, delimit, &saveptr); token != NULL; token = strtok_r(NULL, delimit, &saveptr)) {
		switch (i) {
			case 0: if (glue_strtoi(token, &_N) != 0) { goto err1; } break;
			case 1: if (glue_strtoi(token, &_A) != 0) { goto err1; } break;
			case 2: if (glue_strtoi(token, &_M) != 0) { goto err1; } break;
			case 3: if (glue_strtoi(token, &_T) != 0) { goto err1; } break;
			default:
				printf("invalid N/A/M/T line\n");
				goto err1;
		}
		i++;
	}
	if (_N % (_P - 1) != 0) {
		printf("N is not divisible by (P - 1)\n");
		goto err1;
	}
	// allocate contiguous memory: N x (A+1) | instances * (features + class)
	features = (double *) malloc (_N * (_A + 1) * sizeof(double));
	if (!features) {
		printf("features alloc error\n");
		goto err1;
	}

	/* remaining lines: N lines of input data */
	i = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		//debugf("%s", line);
		// instance sanity check
		if (i >= _N) {
			printf("exceeded total instance count (N):(%d)!\n", _N);
			goto err2;
		}
		// parse 'A features' + '1 class' elements
		j = 0;
		for (token = strtok_r(line, delimit, &saveptr); token != NULL; token = strtok_r(NULL, delimit, &saveptr)) {
			if (j >= (_A + 1)) {
				printf("exceeded total item count (A + 1):(%d) per instance!\n", (_A + 1));
				goto err2;
			}
			// part of the feature vector, parse float
			if (glue_strtod(token, &features[(i * (_A + 1)) + j]) != 0) {
				printf("unable to convert '%s' to float @ instance(%d),item(%d)\n", token, i, j);
				goto err2;
			}
			j++;
		}
		// features sanity check
		if (j != (_A + 1)) {
			printf("not enough features(%d), expected(%d) @ instance(%d)!\n", j, (_A + 1), i);
			goto err2;
		}
		// check if class is '0' or '1'
		c = features[(i * (_A + 1)) + _A];
		if (!((int) c == 0 || (int) c == 1)) {
			printf("invalid class (%f) @ instance(%d)!\n", c, i);
			goto err2;
		}
		i++;
	}
	// instances sanity check
	if (i != _N) {
		printf("not enough instances(%d), expected(%d)!\n", i, _N);
		goto err2;
	}

	if (line)
		free(line);
	fclose(fp);

	// fill pointers
	*P = _P;
	*N = _N;
	*A = _A;
	*M = _M;
	*T = _T;
	*F = features;
	return 0;

	// graceful exit
err2:	free(features);
err1:	fclose(fp);
	if (line)
		free(line);
err0:	return -1;
}
