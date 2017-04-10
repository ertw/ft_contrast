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

void		populateimagedata(FILE *input, t_pgm *pgm)
{
	char	*line;
	size_t	bufsize = 0;
	int	*imagedata;
	int	i = 0;

//	imagedata = ft_memalloc(pgm->width * pgm->height);
	imagedata = malloc(sizeof(int) * pgm->width * pgm->height);
	while (getdelim(&line, &bufsize, (int)' ', input) > 0)
	{
		imagedata[i]  = applycontrast(atoi(line), 0.0008, pgm->maxval);
//		fprintf(output, "%d ", outpixel
//				> pgm->maxval
//				? pgm->maxval
//				: outpixel);
//		printf("%d ", imagedata[i]);
		i++;
	}
	ft_strdel(&line);
	ft_memdel((void*)&imagedata);
	fclose(input);
}

int		main(int ac, char **av)
{
	FILE	*input;
//	int	byte;
	t_pgm	pgm;

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
	populateimagedata(input, &pgm);
//	writeadjustedcontrast(fopen("output.pgm", "ab+"), input, &pgm);
//	while ((byte = getc(file)) >= 0)
//	{
//		ft_putnbr(byte);
//		ft_putchar(' ');
//	}
}
