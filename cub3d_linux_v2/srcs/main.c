/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: idonado <idonado@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/08/21 19:12:50 by idonado       #+#    #+#                 */
/*   Updated: 2021/10/23 21:33:10 by idonado       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>
#include <keycodes_ubuntu.h> 

int	keypressed(int keycode, t_data	*data)
{
	double	old_dir_x;
	double	old_plane_x;
	
	clear_screen(data);
	if (keycode == UP_KEY)
	{
		if(data->map[(int)(data->pos_x + data->dir_x * MOVE_SPEED)][(int)(data->pos_y)] == 0) data->pos_x = data->pos_x + data->dir_x * MOVE_SPEED;
    	if(data->map[(int)(data->pos_x)][(int)(data->pos_y + data->dir_y * MOVE_SPEED)] == 0) data->pos_y = data->pos_y + data->dir_y * MOVE_SPEED;
		render_next_frame(data);
	}
	else if (keycode == DOWN_KEY)
	{
		printf("DOWN\n");
	}
	else if (keycode == LEFT_KEY)
	{
		printf("LEFT\n");
	}
	else if (keycode == RIGHT_KEY)
	{
		printf("RIGHT\n");
	}
	else if (keycode == 65361)
	{
		//both camera direction and camera plane must be rotated
    	old_dir_x = data->dir_x;
    	data->dir_x = data->dir_x * cos(ROTATE_SPEED) - data->dir_y * sin(ROTATE_SPEED);
    	data->dir_y = old_dir_x * sin(ROTATE_SPEED) + data->dir_y * cos(ROTATE_SPEED);
    	old_plane_x = data->plane_x;
    	data->plane_x = data->plane_x * cos(ROTATE_SPEED) - data->plane_y * sin(ROTATE_SPEED);
    	data->plane_y = old_plane_x * sin(ROTATE_SPEED) + data->plane_y * cos(ROTATE_SPEED);
		render_next_frame(data);
	}
	else if (keycode == 65363)
	{
		//both camera direction and camera plane must be rotated
    	old_dir_x = data->dir_x;
    	data->dir_x = data->dir_x * cos(-ROTATE_SPEED) - data->dir_y * sin(-ROTATE_SPEED);
    	data->dir_y = old_dir_x * sin(-ROTATE_SPEED) + data->dir_y * cos(-ROTATE_SPEED);
    	old_plane_x = data->plane_x;
    	data->plane_x = data->plane_x * cos(-ROTATE_SPEED) - data->plane_y * sin(-ROTATE_SPEED);
    	data->plane_y = old_plane_x * sin(-ROTATE_SPEED) + data->plane_y * cos(-ROTATE_SPEED);
		render_next_frame(data);
	}
	return (0);
}

int	**build_map(void)
{
	int	**worldmap;
	int	i;
	int	j;

	worldmap = malloc(sizeof(int *) * 24);
	i = 0;
	while (i < 24)
	{
		j = 0;
		worldmap[i] = malloc(sizeof(int) * 24);
		while (j < 24)
		{
			if (i == 0 || i == 23 || j == 0 || j == 23)
				worldmap[i][j] = 1;
			else
				worldmap[i][j] = 0;
			j++;
		}
		i++;
	}

	worldmap[10][10] = 1;
	worldmap[10][11] = 1;
	worldmap[10][12] = 1;
	worldmap[10][13] = 1;
	worldmap[10][14] = 1;
	worldmap[11][14] = 1;
	worldmap[12][14] = 1;
	worldmap[13][14] = 1;
	worldmap[14][14] = 1;
	worldmap[14][13] = 1;
	worldmap[14][12] = 1;
	worldmap[14][11] = 1;
	worldmap[11][10] = 1;
	worldmap[12][10] = 1;
	worldmap[13][10] = 1;
	worldmap[14][10] = 1;

	return (worldmap);
}

int	render_next_frame(t_data *data)
{


	//used for line size calculation when drawing column by column
	int line_height;
    //calculate lowest and highest pixel to fill in current stripe
    int draw_start;
    int draw_end;

	int	color;
	int		x;

	x = 0;

	while (x < WINWIDTH)
	{
		//calculate ray position and directon
		data->camera_x = 2 * x / (double)WINWIDTH - 1;
		data->ray_dir_x = data->dir_x + data->plane_x * data->camera_x;
		data->ray_dir_y = data->dir_y + data->plane_y * data->camera_x;

		//calculating box in map
		data->map_x = (int)data->pos_x;
		data->map_y = (int)data->pos_y;

		//length of ray from one x or y-side to next x or y-side
		// possible crash ? check abs() function longer repacement
		if (data->ray_dir_x == 0)
			data->delta_dist_x = 1e30;
		else
			data->delta_dist_x = fabs(1 / data->ray_dir_x);
		if (data->ray_dir_y == 0)
			data->delta_dist_y = 1e30;
		else
			data->delta_dist_y = fabs(1 / data->ray_dir_y);

		//delta_dist_x = sqrt(1 + (ray_dir_y * ray_dir_y) / (ray_dir_x * ray_dir_x));
		//delta_dist_y = sqrt(1 + (ray_dir_x * ray_dir_x) / (ray_dir_y * ray_dir_y));

		data->hit = 0;

		//calculate step and initial sideDist
		if (data->ray_dir_x < 0)
		{
			data->step_x = -1;
			data->side_dist_x = (data->pos_x - data->map_x) * data->delta_dist_x;
		}
		else
		{
			data->step_x = 1;
			data->side_dist_x = (data->map_x + 1.0 - data->pos_x) * data->delta_dist_x;
		}
		if (data->ray_dir_y < 0)
		{
			data->step_y = -1;
			data->side_dist_y = (data->pos_y - data->map_y) * data->delta_dist_y;
		}
		else
		{
			data->step_y = 1;
			data->side_dist_y = (data->map_y + 1.0 - data->pos_y) * data->delta_dist_y;
		}
		

		//DDA algorithm
		while (data->hit == 0)
		{
			//jump to next map square, either in x-direction, or in y-direction
        	if (data->side_dist_x < data->side_dist_y)
        	{
        	  data->side_dist_x += data->delta_dist_x;
        	  data->map_x += data->step_x;
        	  data->side = 0;
        	}
        	else
        	{
        	  data->side_dist_y += data->delta_dist_y;
        	  data->map_y += data->step_y;
        	  data->side = 1;
        	}
        	//Check if ray has hit a wall
        	if (data->map[data->map_x][data->map_y] > 0)
				data->hit = 1;
		}

		//Calculate distance projected on camera direction 
      	if(data->side == 0)
			data->perp_wall_dist = (data->side_dist_x - data->delta_dist_x);
      	else
			data->perp_wall_dist = (data->side_dist_y - data->delta_dist_y);
		
		//Calculate height of line to draw on screen
      	line_height = (int)(WINHEIGHT / data->perp_wall_dist);

      	//calculate lowest and highest pixel to fill in current stripe
      	draw_start = (-line_height / 2) + (WINHEIGHT / 2);
      	if(draw_start < 0)
			draw_start = 0;
      	draw_end = (line_height / 2) + (WINHEIGHT / 2);
      	if(draw_end >= WINHEIGHT)
			draw_end = WINHEIGHT - 1;

		color = 0x00FF0055;
		//give x and y sides different brightness
    	if (data->side == 1)
			{color = 0x005500FF;}

	printf("drawstart%d\ndrawend%d\n\n", draw_start, draw_end);
		//draw wall
		if (0 < draw_start && draw_start >= 0)draw_vertical_line(data, x, 0, draw_start, 0x00FFFFF0);
		//draw wall
		if (draw_start < draw_end && draw_end >= 0 && draw_start >= 0)draw_vertical_line(data, x, draw_start, draw_end, color);
		//draw floor
		if (draw_end < WINHEIGHT && draw_end >= 0) draw_vertical_line(data, x, draw_end, (WINHEIGHT), 0x00FF0000);

		//color where it hits
		my_mlx_pixel_put(data->img, x, draw_end, 0x00000000);

		x++;
	}


	mlx_put_image_to_window(data->mlx->mlx, data->mlx->mlx_window, data->img->img, 0, 0);
	print_map(data);
	return (0);
}

int	game_loop(t_data *data)
{
	usleep(50);
	(void)data;
	return (0);
}

int	main(void)
{
	t_data	*data;
	
	data = malloc(sizeof(t_data));
	data->mlx = malloc(sizeof(t_mlx_vars));
	data->img = malloc(sizeof(t_img_data));

	data->map = build_map();
	print_map(data);

	//initializing mlx and image
	data->mlx->mlx = mlx_init();
	data->mlx->mlx_window = mlx_new_window(data->mlx->mlx, WINWIDTH, WINHEIGHT, "cub3d");
	data->img->img = mlx_new_image(data->mlx->mlx, WINWIDTH, WINHEIGHT);
	data->img->addr = mlx_get_data_addr(data->img->img, &data->img->bits_per_pixel, &data->img->line_length, &data->img->endian);

	//giving default position coordinates
	data->pos_x = 22;
	data->pos_y = 12;

	//direction coordinate
	data->dir_x = -1;
	data->dir_y = 0;

	//camera plane
	data->plane_x = 0;
	data->plane_y = 0.66;


	// key hooks and game loop
	mlx_hook(data->mlx->mlx_window, 2, 1L << 0, keypressed, data);
	render_next_frame(data);
	mlx_loop_hook(data->mlx->mlx, game_loop, data);

	mlx_loop(data->mlx->mlx);
	return (0);
}