#include "ui.h"
#include <cstdio>
#include <cmath>
#include <string> 

#define SCREEN_HOR 640
#define SCREEN_VER 480
#define PI 3.14159265


lv_obj_t* canvas;
lv_obj_t* layer2_leftSideBox;
lv_obj_t* layer2_rightSideBox;
lv_obj_t* layer2_centerBox;
lv_obj_t* layer2_leftSideBox_line; 
lv_obj_t* layer2_rightSideBox_line;
lv_obj_t* layer2_leftArrow_line;
lv_obj_t* layer2_rightArrow_line;

static lv_style_t style_sky;
static lv_style_t style_side;
static lv_style_t style_ground;
static lv_style_t style_white_line;
static lv_style_t style_black_line;
static lv_style_t style_small_font;

static lv_point_t side_linePoint[] = { {0, 0}, {0, SCREEN_VER/2} };
static lv_point_t left_arrowPoint[] = { {0, 0}, {SCREEN_VER / 20, 0},{SCREEN_VER / 20, SCREEN_VER/40} };
static lv_point_t right_arrowPoint[] = { {0, SCREEN_VER / 40}, {0, 0},{SCREEN_VER / 20,0} };
static lv_point_t short_linePoint[] = { {0, 0}, {SCREEN_VER / 40, 0} };
static lv_point_t mid_linePoint[] = { {0, 0}, {SCREEN_VER / 20, 0} };
//static lv_point_t long_linePoint[] = { {0, SCREEN_VER / 5 * tan(abs(45) * PI / 180)}, {SCREEN_VER / 5, SCREEN_VER / 5 * tan(abs(45) * PI / 180)} };

/**
 * @brief init style
*/
void init_style() {

    lv_style_init(&style_sky);
    lv_style_set_bg_color(&style_sky, lv_color_hex(0x1353EA));

    lv_style_init(&style_side);
    lv_style_set_bg_color(&style_side, lv_color_hex(0x9fc5e8));
    lv_style_set_border_color(&style_side, lv_color_hex(0x000000));
    lv_style_set_bg_opa(&style_side, LV_OPA_50);

    lv_style_init(&style_ground);
    lv_style_set_bg_color(&style_ground, lv_color_hex(0x7f6000));
    lv_style_set_border_color(&style_ground, lv_color_hex(0x000000));

    lv_style_init(&style_white_line);
    lv_style_set_line_width(&style_white_line, 3);
    lv_color_t white = lv_color_make(255, 255, 255);
    lv_style_set_line_color(&style_white_line, white);
    lv_style_set_line_rounded(&style_white_line, true);

    lv_style_init(&style_black_line);
    lv_style_set_line_width(&style_black_line, 5);
    lv_color_t black = lv_color_make(0, 0, 0);
    lv_style_set_line_color(&style_black_line, black);
    lv_style_set_line_rounded(&style_black_line, true);

    lv_style_init(&style_small_font);
    lv_style_set_text_font(&style_small_font, &lv_font_montserrat_12);
}

/**
 * @brief refresh-base_canvas
 * @param angle 
 * @param  
 * @param mid_x 
 * @param mid_y 
*/
static void canvas_fresh(bool isNg,double angle, size_t mid_x, size_t mid_y) {
    for (size_t x = 0; x < SCREEN_HOR; x++)
    {

        for (size_t y = 0; y < SCREEN_VER; y++)
        {
            if (!isNg) {
                if (x < mid_x) {
                    if (y < mid_y) {
                        lv_canvas_set_px_color(canvas, x, y, lv_color_hex(0x1353EA));
                    }
                    else if (y >= mid_y && y < mid_y + (mid_x - x) * tan(angle)) {
                        lv_canvas_set_px_color(canvas, x, y, lv_color_hex(0x1353EA));
                    }
                    else {
                        lv_canvas_set_px_color(canvas, x, y, lv_color_hex(0x7f6000));
                    }
                }
                else {
                    if (y < mid_y - (x - mid_x) * tan(angle)) {
                        lv_canvas_set_px_color(canvas, x, y, lv_color_hex(0x1353EA));
                    }
                    else {
                        lv_canvas_set_px_color(canvas, x, y, lv_color_hex(0x7f6000));
                    }
                }
            }
            else {
                if (x > mid_x) {
                    if (y > mid_y && y > mid_y + (x-mid_x) * tan(angle)) {
                        lv_canvas_set_px_color(canvas, x, y, lv_color_hex(0x7f6000));
                    }
                    else {
                        lv_canvas_set_px_color(canvas, x, y, lv_color_hex(0x1353EA));
                    }
                }
                else {
                    if (y > mid_y - (mid_x-x) * tan(angle)) {
                        lv_canvas_set_px_color(canvas, x, y, lv_color_hex(0x7f6000));
                    }
                    else {
                        lv_canvas_set_px_color(canvas, x, y, lv_color_hex(0x1353EA));
                    }
                }
            }

        }
    }
}

/**
 * @brief init base_canvas
*/
void init_sg(size_t xbias, size_t ybias, double deg) {

    static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(SCREEN_HOR, SCREEN_VER)];

    canvas = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(canvas, cbuf, SCREEN_HOR, SCREEN_VER, LV_IMG_CF_TRUE_COLOR);
    lv_obj_center(canvas);
    lv_canvas_fill_bg(canvas, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_OPA_COVER);
    size_t mid_x = SCREEN_HOR / 2 + xbias;
    size_t mid_y = SCREEN_VER / 2 + ybias;

    double angle = abs(deg) * PI / 180;

    canvas_fresh(deg<0?true:false,angle, mid_x, mid_y);

}

/**
 * @brief init_leftSideBox
*/
void init_leftSideBox() {

    layer2_leftSideBox = lv_obj_create(canvas);
    lv_obj_add_style(layer2_leftSideBox, &style_side, 0);
    lv_obj_set_size(layer2_leftSideBox, SCREEN_HOR/10, SCREEN_VER/2+20);
    lv_obj_align(layer2_leftSideBox, LV_ALIGN_LEFT_MID, SCREEN_HOR / 10, 0);
    lv_obj_clear_flag(layer2_leftSideBox, LV_OBJ_FLAG_SCROLLABLE);

    layer2_leftSideBox_line = lv_line_create(layer2_leftSideBox);
    lv_line_set_points(layer2_leftSideBox_line, side_linePoint, 2);     /*Set the points*/
    lv_obj_add_style(layer2_leftSideBox_line, &style_white_line, 0);
    lv_obj_align(layer2_leftSideBox_line, LV_ALIGN_RIGHT_MID, 15, 0);
}

/**
 * @brief init_rightSideBox
*/
void init_rightSideBox() {

    layer2_rightSideBox = lv_obj_create(canvas);
    lv_obj_add_style(layer2_rightSideBox, &style_side, 0);
    lv_obj_set_size(layer2_rightSideBox, SCREEN_HOR / 10+20, SCREEN_VER / 2+20);
    lv_obj_align(layer2_rightSideBox, LV_ALIGN_RIGHT_MID, -(SCREEN_HOR / 10), 0);
    lv_obj_clear_flag(layer2_rightSideBox, LV_OBJ_FLAG_SCROLLABLE);

    layer2_rightSideBox_line = lv_line_create(layer2_rightSideBox);
    lv_line_set_points(layer2_rightSideBox_line, side_linePoint, 2);     /*Set the points*/
    lv_obj_add_style(layer2_rightSideBox_line, &style_white_line, 0);
    lv_obj_align(layer2_rightSideBox_line, LV_ALIGN_LEFT_MID, -13, 0);

}

/**
 * @brief init_arrow
*/
void init_arrow() {
    layer2_leftArrow_line = lv_line_create(canvas);
    layer2_rightArrow_line = lv_line_create(canvas);

    lv_line_set_points(layer2_leftArrow_line, left_arrowPoint, 3);     /*Set the points*/
    lv_obj_add_style(layer2_leftArrow_line, &style_black_line, 0);
    lv_obj_align(layer2_leftArrow_line, LV_ALIGN_CENTER, -SCREEN_HOR*0.08, 10);

    lv_line_set_points(layer2_rightArrow_line, right_arrowPoint, 3);     /*Set the points*/
    lv_obj_add_style(layer2_rightArrow_line, &style_black_line, 0);
    lv_obj_align(layer2_rightArrow_line, LV_ALIGN_CENTER, SCREEN_HOR * 0.08, 10);

}

/**
 * @brief init_leftLineMark
 * @param step 
 * @param shift_px 
 * @param bias 
*/
void init_leftLineMark(int step,int shift_px, int bias) {
    for (int i = 0; i <= (SCREEN_VER / 2) / 20; i++) {
        lv_obj_t* line;
        line = lv_line_create(layer2_leftSideBox);
        lv_line_set_points(line, short_linePoint, 2);     /*Set the points*/
        lv_obj_add_style(line, &style_white_line, 0);
        lv_obj_align(line, LV_ALIGN_TOP_MID, 20, i * 20 - (shift_px % 10)-10);

        if (((SCREEN_VER / 2) / 20 / 2 + bias - i) * step >= 0) {
            lv_obj_t* label = lv_label_create(layer2_leftSideBox);
            lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
            lv_label_set_recolor(label, true);                      /*Enable re-coloring by commands in the text*/
            lv_label_set_text_fmt(label, "#ffffff %d #", ((SCREEN_VER / 2) / 20 / 2 + bias - i) * step);
            lv_obj_set_width(label, 150);  /*Set smaller width to make the lines wrap*/
            lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_RIGHT, 0);
            lv_obj_align(label, LV_ALIGN_TOP_LEFT, -125, i * 20 - (shift_px%10) -15);
        }
    }

}


void init_rightLineMark(int step,int shift_px,int bias) {
    for (int i = 0; i <= (SCREEN_VER / 2) / 20; i++) {
        lv_obj_t* line;
        line = lv_line_create(layer2_rightSideBox);
        lv_line_set_points(line, short_linePoint, 2);     /*Set the points*/
        lv_obj_add_style(line, &style_white_line, 0);
        lv_obj_align(line, LV_ALIGN_TOP_MID, -30, i * 20 - (shift_px % 10) - 10);

        if (((SCREEN_VER / 2) / 20 / 2 + bias - i) * step >= 0) {
            lv_obj_t* label = lv_label_create(layer2_rightSideBox);
            lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
            lv_label_set_recolor(label, true);                      /*Enable re-coloring by commands in the text*/
            lv_label_set_text_fmt(label, "#ffffff %d #", ((SCREEN_VER / 2) / 20 / 2 + bias - i) * step);
            lv_obj_set_width(label, 150);  /*Set smaller width to make the lines wrap*/
            lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, 0);
            lv_obj_align(label, LV_ALIGN_TOP_LEFT, 5, i * 20 - (shift_px % 10) - 15);
        }
    }

}

static lv_color_t layer2_cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(SCREEN_HOR / 2, SCREEN_VER)];

void init_centerBox() {
    layer2_centerBox = lv_canvas_create(canvas);
    lv_canvas_set_buffer(layer2_centerBox, layer2_cbuf, SCREEN_HOR / 2, SCREEN_VER, LV_IMG_CF_TRUE_COLOR_ALPHA);
    
    lv_obj_set_size(layer2_centerBox, SCREEN_HOR /2, SCREEN_VER );
    lv_obj_align(layer2_centerBox, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(layer2_centerBox, LV_OBJ_FLAG_SCROLLABLE);
}


void init_centerLineMark(int step, int x_bias, int y_bias, double deg) {

    lv_canvas_fill_bg(layer2_centerBox, lv_color_black(), LV_OPA_0);

    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    line_dsc.color = lv_color_hex(0xffffff);
    line_dsc.width = 3;

    int half_line_length = 70;
    int ng = deg < 0 ? -1 : 1;

    lv_point_t angle_linePoint[] = { {SCREEN_HOR / 4 - half_line_length *  cos(abs(deg) * PI / 180) + x_bias,SCREEN_VER / 2 + y_bias +  half_line_length * sin(abs(deg) * PI / 180)  * ng  },
            {SCREEN_HOR / 4 + half_line_length * cos(abs(deg) * PI / 180)  + x_bias,  SCREEN_VER / 2 + y_bias -  half_line_length * sin(abs(deg) * PI / 180)  * ng  } };

    lv_canvas_draw_line(layer2_centerBox, angle_linePoint, 2, &line_dsc); 

    for (int i = 0; i <  step; i++) {

        int half_line_length = 50; 

        lv_point_t angle_linePoint[] = { {SCREEN_HOR / 4 - half_line_length * cos(abs(deg) * PI / 180) + x_bias,SCREEN_VER / step / 2 + (i * SCREEN_VER / step) + y_bias + half_line_length * sin(abs(deg) * PI / 180)  * ng },
            {SCREEN_HOR / 4 + half_line_length * cos(abs(deg) * PI / 180) + x_bias,  (i * SCREEN_VER / step) + SCREEN_VER / step / 2 + y_bias -  half_line_length * sin(abs(deg) * PI / 180)  * ng  } };

        lv_canvas_draw_line(layer2_centerBox, angle_linePoint, 2, &line_dsc);

        lv_draw_label_dsc_t label_dsc;
        lv_draw_label_dsc_init(&label_dsc); 
        label_dsc.color = lv_color_hex(0xffffff);
        //lv_canvas_draw_text(layer2_centerBox, SCREEN_HOR / 4 - 75 + x_bias, SCREEN_VER / step / 2 + (i * SCREEN_VER / step) + y_bias + half_line_length * sin(abs(deg) * PI / 180) * ng, 30, &label_dsc, std::to_string((step / 2 - i > 0 ? step / 2 - i : step / 2 - i - 1) * step).c_str());
        //lv_canvas_draw_text(layer2_centerBox, SCREEN_HOR / 4 + 60 + x_bias, (i * SCREEN_VER / step) + SCREEN_VER / step / 2 + y_bias - half_line_length * sin(abs(deg) * PI / 180) * ng, 30, &label_dsc, std::to_string((step / 2 - i > 0 ? step / 2 - i : step / 2 - i - 1) * step).c_str());

        //lv_obj_t* line;
        //line = lv_line_create(layer2_centerBox);
        //lv_line_set_points(line, angle_linePoint, 2);     /*Set the points*/
        //lv_obj_add_style(line, &style_white_line, 0);
        //lv_obj_align(line, LV_ALIGN_TOP_MID, 0, (i + 1) * (SCREEN_VER / step) - (SCREEN_VER / 12) - shift_px);

        //lv_obj_t* left_label = lv_label_create(layer2_centerBox);
        //lv_label_set_long_mode(left_label, LV_LABEL_LONG_WRAP);     
        //lv_label_set_recolor(left_label, true);                      
        //lv_label_set_text_fmt(left_label, "#ffffff %d #", (step / 2 - i > 0 ? step / 2 - i : step / 2 - i - 1) * step);
        //lv_obj_set_width(left_label, 150);  
        //lv_obj_set_style_text_align(left_label, LV_TEXT_ALIGN_CENTER, 0);
        //lv_obj_align(left_label, LV_ALIGN_TOP_LEFT, 0, (i + 1) * (SCREEN_VER / step) - (SCREEN_VER / 12) - 6 + (deg >= 0 ? SCREEN_VER / 5 * tan(abs(deg) * PI / 180) : 0) + 15 + y_bias);
        //lv_obj_add_style(left_label, &style_small_font, 0);

        //lv_obj_t* right_label = lv_label_create(layer2_centerBox);
        //lv_label_set_long_mode(right_label, LV_LABEL_LONG_WRAP);     
        //lv_label_set_recolor(right_label, true);                      
        //lv_label_set_text_fmt(right_label, "#ffffff %d #", (step / 2 - i > 0 ? step / 2 - i : step / 2 - i - 1) * step);
        //lv_obj_set_width(right_label, 150);  
        //lv_obj_set_style_text_align(right_label, LV_TEXT_ALIGN_CENTER, 0);
        //lv_obj_align(right_label, LV_ALIGN_TOP_RIGHT, 0, (i + 1) * (SCREEN_VER / step) - (SCREEN_VER / 12) - 6 + (deg >= 0 ? SCREEN_VER / 5 * tan(abs(deg) * PI / 180) : 0) + 15 + y_bias);
        //lv_obj_add_style(right_label, &style_small_font, 0);
    }

    /*for (int i = 0; i <= SCREEN_VER / 40; i++) {
        lv_obj_t* mid_line;
        mid_line = lv_line_create(layer2_centerBox);
        lv_line_set_points(mid_line, mid_linePoint, 2);     
        lv_obj_add_style(mid_line, &style_white_line, 0);
        lv_obj_align(mid_line, LV_ALIGN_TOP_MID, 0, (i + 18) * 10);

        if (i < SCREEN_VER / 40) {
            lv_obj_t* short_line;
            short_line = lv_line_create(layer2_centerBox);
            lv_line_set_points(short_line, short_linePoint, 2);     
            lv_obj_add_style(short_line, &style_white_line, 0);
            lv_obj_align(short_line, LV_ALIGN_TOP_MID, 0, (i + 18) * 10 + 5);
        }
    }*/
}

/**
 * @brief unused-test
 * @param  
*/
void transformer_center(int deg) {
    static lv_color_t layer2_cbuf_tmp[LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(SCREEN_HOR / 2, SCREEN_VER)];
    memcpy(layer2_cbuf_tmp, layer2_cbuf, sizeof(layer2_cbuf_tmp));
    lv_img_dsc_t img;
    img.data = (uint8_t *)layer2_cbuf_tmp;
    img.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
    img.header.w = SCREEN_HOR / 2;
    img.header.h = SCREEN_VER;
    lv_canvas_fill_bg(layer2_centerBox, lv_color_black(), LV_OPA_0);
    lv_canvas_transform(layer2_centerBox, &img, deg, LV_IMG_ZOOM_NONE, 0, 0, SCREEN_HOR / 4, SCREEN_VER / 2, false);
}

/**
 * @brief anim_leftbox
 * @param
*/
static void anim_left_cb(void* var, int32_t v)
{
    lv_obj_del(layer2_leftSideBox);
    init_leftSideBox();
    init_leftLineMark(50, v, v);
    
}

/**
 * @brief anim_rightbox
 * @param
*/
static void anim_right_cb(void* var, int32_t v)
{
    lv_obj_del(layer2_rightSideBox);
    init_rightSideBox();
    init_rightLineMark(500, v, v);

}

/**
 * @brief anim_canvas
 * @param
*/
static void anim_canvas_cb(void* var, int32_t deg)
{
    size_t mid_x = SCREEN_HOR / 2 + deg;
    size_t mid_y = SCREEN_VER / 2 + deg;
    double angle = abs(deg) * PI / 180;
    canvas_fresh(deg < 0 ? true : false, angle, mid_x, mid_y);
    //lv_obj_del(layer2_centerBox);
    //init_centerBox();
    //init_centerLineMark(10, deg, deg, deg);
    //transformer_center(deg * 10);
}


void ui(void) {

    init_style();

    init_sg(0,0,0);

    init_leftSideBox();

    init_rightSideBox();

    init_arrow();

    init_centerBox();

    init_leftLineMark(50,0,0);

    init_rightLineMark(500,0,0);

    init_centerLineMark(10, 0, 0,0);

    lv_anim_t anim_right;
    lv_anim_init(&anim_right);
    lv_anim_set_var(&anim_right, NULL);
    lv_anim_set_values(&anim_right, 0, 100);
    lv_anim_set_time(&anim_right, 3000);
    lv_anim_set_exec_cb(&anim_right, anim_right_cb);
    lv_anim_set_path_cb(&anim_right, lv_anim_path_ease_in_out);
    lv_anim_set_repeat_delay(&anim_right, 500);
    lv_anim_set_repeat_count(&anim_right, LV_ANIM_REPEAT_INFINITE);
    //lv_anim_start(&anim_right);


    lv_anim_t anim_left;
    lv_anim_init(&anim_left);
    lv_anim_set_var(&anim_left, NULL);
    lv_anim_set_values(&anim_left, 0, 100);
    lv_anim_set_time(&anim_left, 3000);
    lv_anim_set_exec_cb(&anim_left, anim_left_cb);
    lv_anim_set_path_cb(&anim_left, lv_anim_path_ease_in_out);
    lv_anim_set_repeat_delay(&anim_left, 500);
    lv_anim_set_repeat_count(&anim_left, LV_ANIM_REPEAT_INFINITE);
    //lv_anim_start(&anim_left);

    lv_anim_t anim_canvas;
    lv_anim_init(&anim_canvas);
    lv_anim_set_var(&anim_canvas, NULL);
    lv_anim_set_values(&anim_canvas, -90,90);
    lv_anim_set_time(&anim_canvas, 5000);
    lv_anim_set_exec_cb(&anim_canvas, anim_canvas_cb);
    lv_anim_set_path_cb(&anim_canvas, lv_anim_path_ease_in_out);
    lv_anim_set_repeat_delay(&anim_canvas, 1000);
    lv_anim_set_repeat_count(&anim_canvas, LV_ANIM_REPEAT_INFINITE);
    //lv_anim_start(&anim_canvas);

}
