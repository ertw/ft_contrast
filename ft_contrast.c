#include "ft_contrast.h"

typedef struct		s_pgm
{
	int		width;
	int		height;
	int		maxval;
}			t_pgm;

char		*parseheader(FILE *file, t_pgm *pgm)
{
	char	*line;
	char	*token;
	char	*tofree;
	size_t	bufsize = 0;

	pgm->width = 0;
	line = NULL;
	getline(&line, &bufsize, file);
	ft_strdel(&line);
//	getline(&line, &bufsize, file);
//	ft_strdel(&line);
	getline(&line, &bufsize, file);
	tofree = line;
	token = strsep(&line, " ");
	pgm->width = atoi(token);
	token = strsep(&line, " ");
	pgm->height = atoi(token);
	ft_strdel(&tofree);
	getline(&line, &bufsize, file);
	pgm->maxval = atoi(line);
	ft_strdel(&line);
	return (0);
}

void		writeheader(FILE *file, t_pgm *pgm)
{
	fprintf(file, "P2\n");
	fprintf(file, "# Created by ft_contrast\n");
	fprintf(file, "%d %d\n", pgm->width, pgm->height);
	fprintf(file, "%d\n", pgm->maxval);
	fclose(file);
}

int		applycontrast(int pixel, double contrast, int maxgrey)
{
	return (pixel * contrast * maxgrey);
}

// resultPixel = pixelColor ∗ (contrastLevel) ∗ maxGreyLevel
void		writeadjustedcontrast(FILE *output, FILE *input, t_pgm *pgm)
{
	char	*line;
	size_t	bufsize = 0;
	int	outpixel;

	while (getdelim(&line, &bufsize, (int)' ', input) > 0)
	{

		outpixel = applycontrast(atoi(line), 0.0008, pgm->maxval);
		fprintf(output, "%d ", outpixel
				> pgm->maxval
				? pgm->maxval
				: outpixel);
	}
	ft_strdel(&line);
	fclose(output);
	fclose(input);
}

typedef struct		s_input
{
	t_pgm		*pgm;
	FILE		*input;
	int		*imgdata;
	int		pos;
	int		partiallen;
}			t_input;

int		*writetoimgdata(FILE *input, t_pgm *pgm)
{
	char		*line;
	size_t		bufsize = 0;
	int		*imgdata;
	int		i = 0;
	char		*tofree;

	line = malloc(1);
	tofree = line;
	imgdata = malloc(sizeof(int*) * pgm->width * pgm->height);
	while ((getdelim(&line, &bufsize, (int)' ', input) > 0))
	{
		imgdata[i] = atoi(line);
		i++;
	}
	free(tofree);
	fclose(input);
	return (imgdata);
}

void		mutateimgdata(t_input *inputcont)
{
	int	i;
	int	oldpixel;

	i = inputcont->pos;
	while (i < inputcont->partiallen)
	{
		oldpixel = inputcont->imgdata[i];
		applycontrast(oldpixel, 0.0008, inputcont->pgm->maxval);
		i++;
	}
}

int		main(int ac, char **av)
{
	FILE		*input;
	t_pgm		pgm;
	pthread_t	thread1;
	pthread_t	thread2;
	t_input		inputcont;
	int		*imgdata;

	if (!ac)
	{
		;
	}
	input = fopen(av[1], "r");
	parseheader(input, &pgm);
	ft_putnbr(pgm.width);
	ft_putchar('\n');
	ft_putnbr(pgm.width);
	ft_putchar('\n');
	ft_putnbr(pgm.maxval);
	ft_putchar('\n');
	writeheader(fopen("output.pgm", "ab+"), &pgm);
	inputcont.pgm = &pgm;
	inputcont.input = input;
	imgdata = writetoimgdata(input, &pgm);
	inputcont.partiallen = (pgm.width * pgm.height) / 2;
	inputcont.imgdata = imgdata;
	inputcont.pos = 0;
	pthread_create(&thread1, NULL, (void*)mutateimgdata, (void*)&inputcont);
	inputcont.pos = (pgm.width * pgm.height) / 2;
	pthread_create(&thread2, NULL, (void*)mutateimgdata, (void*)&inputcont);
	pthread_join(thread1, NULL);
	free(imgdata);
}
