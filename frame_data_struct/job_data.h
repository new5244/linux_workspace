/*!
 * \file job_data.h
 * \brief
 *
 *
 * created by Kim Youngmin (ymkim@huvitz.com)
 * creation date : �� 5 30  Korea Standard Time 2008
 *
*/
#ifndef _JOB_DATA_H_
#define _JOB_DATA_H_

#include <llist.h>
#include <epc_type_def.h>
#include <drill_data.h>

#define JOB_NO_STR_SIZE 128 // utf8 string ó���� ���� 128 bytes �� Ȯ��
#define GUI_JOB_NO_SIZE 16 // GUI ���� �ν��ϴ� job number string size (���� & �빮��)

// GUI �� bevel position index
#define GUI_BP_PERCENT EPC_BP_PERCENT
#define GUI_BP_FRONT EPC_BP_FRONT
#define GUI_BP_REAR EPC_BP_REAR
#define GUI_BP_BC EPC_BP_BC
#define GUI_BP_FC MAX_EPC_BP_IDX
#define MAX_GUI_BP_IDX (MAX_EPC_BP_IDX+1)

enum edging_process_idx {
	EP_JOB_START,
	EP_JOB_END,
	EP_EDGING_START,
	EP_EDGING_END,
	EP_FEELING_START,
	EP_FEELING_END,
	EP_ROUGHING_START,
	EP_ROUGHING_END,
	EP_FINISHING_START,
	EP_FINISHING_END,
	EP_POLISHING_START,
	EP_POLISHING_END,
	EP_GROOVING_START,
	EP_GROOVING_END,
	EP_CHAMFERING_START,
	EP_CHAMFERING_END,
	EP_DRILLING_START,
	EP_DRILLING_END,
	EP_RETOUCHING_START,
	EP_RETOUCHING_END,
	EP_CHAMFERING_FRONT,
	EP_CHAMFERING_REAR,
	EP_EDGING_STOP,
	EP_DRILLING_STOP,
	EP_POS_IDX,
	EP_ROTATION_IDX,

	MAX_EDGING_PROCESS_IDX
};

enum frame_data_source_device_idx {
	TR_CFR_4000,
	TR_CFR_3000,
	TR_CAB_4000,
	TR_CPE_4000,
	TR_CPE_3000,
	TR_HPE_7000,
	TR_OTHER_TRACER,
	TR_OTHER_BLOCKER,
	TR_HBK_7000,
	TR_NEX_TRACER,
	TR_NEX_BLOCKER,
	TR_NEX_MAIN,
	TR_NEX_DRILL,
	TR_UNKNOWN,

	MAX_TR_IDX
};

// alloc_frame_raw_data() ���� ����ϴ� flag
#define FRD_COUNT_MASK 0x0000ffff
#define FRD_SALLOC_R 0x10000000
#define FRD_SALLOC_A 0x20000000
#define FRD_SALLOC_Z 0x40000000
#define FRD_USE_Z 0x01000000 // Z �� ������ ��� 
#define FRD_SALLOC_X 0x02000000
#define FRD_SALLOC_Y 0x04000000

/// frame raw data
/// array ��� ������ ���, �ʿ��� ��ū �Ҵ��ؼ� ����.
/// �޸� �Ҵ� ������ flag �� ����Ѵ�.
struct frame_raw_data {
	int flag;
	int record_count;
	float *r;
	float *a;
	float *z;
};

/// GUI �� x, y  data
struct xy_data {
	int flag;
	int record_count;
	float *x;
	float *y;
};

/// boxing center ���� (xy ��ǥ)
struct boxing_center_info {
	float center_x;
	float center_y;
	float max_x;
	float max_y;
	float min_x;
	float min_y;
	int max_x_idx;
	int max_y_idx;
	int min_x_idx;
	int min_y_idx;
};

/// digital pattern ���� ��
struct digital_pattern_data {
	float down;
	float updown;
	float leftright;
	float radius;
	float percent;
	float rotation;
};


// retouch data
struct retouch_data {
	float size;
	float prev_size;
	int polish;
	float grooving_depth;
};

// ����ġ ��� ���Խ� �����ɼ��� ����Ǿ����� Ȯ���ϱ� ����
struct retouch_check_data {
	float bridge_size;
	int vision_mode;
	float bifocal_offset_h;
	float bifocal_offset_v;
	float bifocal_dia;
	float single_pd;
	float delta_y;
	int blocking_mode;
	int fragile_mode; 
	int lens_material;
	int edging_type;
	int pos_type;
	float pos_value;
	float size;
	int polish;
	int sb_front;
	int sb_rear;
	float grooving_depth;
	float grooving_width;

	struct boxing_center_info bci;
	// �ʵ尡 �̰͸� ������ �ɱ�????
};

/// �Ȱ��� ������ �ٽ� �ڷ� ����
typedef struct job_data {
	struct list_head list;

	// frame data �� ���� structure �� �и� 
	struct frame_raw_data frame[MAX_FRAME_SIDE];
	struct frame_raw_data org_frame[MAX_FRAME_SIDE];

	// ���� bci structure �״�� ��� 
	struct boxing_center_info bci[MAX_FRAME_SIDE_IDX];

	char job_no[JOB_NO_STR_SIZE]; // job number

	int frame_type; // ������  (metal, plastic, optyl, pattern)
	float frame_curve[MAX_FRAME_SIDE_IDX]; // �� Ŀ�� 
	float circumference[MAX_FRAME_SIDE_IDX];
	float bridge_size; // bridge size 
	float fpd; // ����� Ʈ���̼����� �׸� �о��� ��츦 ����� �ʵ�, ��� ����� bridge_size �� �������� �Ѵ�. 

	// layout option
	int vision_mode; // vision mode  ( far, bifocal, ...)
	float bifocal_offset_h[MAX_FRAME_SIDE_IDX];
	float bifocal_offset_v[MAX_FRAME_SIDE_IDX];
	float bifocal_dia[MAX_FRAME_SIDE_IDX];  // bifocal ����

	//int pd_type; // pd, single_pd : hex ������ ������� �ʴ´�. 
	float pd; // ��� pd 
	float single_pd[MAX_FRAME_SIDE_IDX]; // �ܾ� PD 
	// float delta_x[MAX_FRAME_SIDE_IDX]; delta x �� ���α׷����� ����Ѵ�
	
	int oh_type[MAX_FRAME_SIDE_IDX]; // oh type : delta Y , boxed h , mixed h
	float oh_value[MAX_FRAME_SIDE_IDX][MAX_EPC_OH_TYPE_IDX]; // oh �� (oh_type ����)
	float delta_y[MAX_FRAME_SIDE_IDX]; // delta y for internal use

	int blocking_mode; // boxing/optical

	// unused, ������ dp ȭ�鿡�� ó��
	struct digital_pattern_data dp[MAX_FRAME_SIDE_IDX]; // digital pattern �� 

	// edging option
	int fragile_mode; // off/on
	int lens_material[MAX_FRAME_SIDE_IDX]; // pl, pc, hi, gl, tri
	int edging_type[MAX_FRAME_SIDE_IDX]; // rimless, bevel, grooving

	// bevel/grooving ���� 
	int pos_type[MAX_FRAME_SIDE_IDX]; // percent, front, rear, bc, �갢 FC ��� 
	float pos_value[MAX_FRAME_SIDE_IDX][MAX_GUI_BP_IDX]; // bevel ��ġ �� (type  ����)

	float size[MAX_FRAME_SIDE_IDX]; // ���� ũ�� 
	int polish[MAX_FRAME_SIDE_IDX];// off/ on
	int sb_front[MAX_FRAME_SIDE_IDX]; // off/on
	int sb_rear[MAX_FRAME_SIDE_IDX]; // off/small/large
	float grooving_depth[MAX_FRAME_SIDE_IDX];
	float grooving_width[MAX_FRAME_SIDE_IDX];


	// gui ���� 
	int current_side; // ���� ���� side
	int edging_done[MAX_FRAME_SIDE_IDX]; // ���� �Ϸ� ���� 
	int frame_change[MAX_FRAME_SIDE_IDX]; // frame change flag
	int clamp_pressure;
	struct retouch_data retouch[MAX_FRAME_SIDE_IDX]; // ����ġ ������ 

	struct xy_data xydata[MAX_FRAME_SIDE_IDX]; // frame data x, y ��ȯ 
	int job_flag; // ���⿡�� frame data source device flag �� ��ϵȴ�.
#define JF_DEVICE_MASK 0x0f
#define JF_RL_SEP 0x10 // R/L ���� ���� (rl_sync �ʵ� ����, R/L ���� ������ default �� ó���ϱ� ����)
#define JF_PROTECTED 0x20 // layout �ʵ� ��ȣ (auto blocker data)
#define JF_SEP_WARN 0x40 // r/l sync off ������ �� ��� �޽��� �ѹ� �ѷ���
#define JF_ON_EDGING 0x80 // ������ ���۵�
#define JF_RETOUCH 0x0100 // ����ġ ��� 
#define JF_HOLE_EDIT 0x0200 // hole edit ��� 
#define JF_FPD 0x0400 // bridge ǥ�� 0, fpd ǥ�� 1 
#define JF_DIGITAL_PATTERN 0x0800 // digital pattern mode
#define JF_FRAME_CHANGE_LENS 0x1000 // frame change �� ���� lens �������� ��Ÿ��
#define JF_DRILL_RETOUCH 0x2000 // drilling retouch mode
#define JF_R_DRILLED 0x4000 // Right drilling completed // 20090928 drilling flag �� job flag �� ��ü�Ϸ��� �غ�
#define JF_L_DRILLED 0x8000 // Left drilling completed 
#define JF_DCURVE 0x010000 // drill demo lens curve ���� 
#define JF_DRILL_ADJ_DATA 0x020000 // drilling ���� ������ ǥ��
#define JF_DEF_SIDE_CHECKED 0x040000 // �⺻ ���� ���̵� �˻� �Ϸ� flag
#define JF_DRILL_WRAP 0x080000 // Drill wrap mode on/off
#define JF_HI_POLISH 0x100000 // high glossy polishing
#define JF_DQ_HI 0x200000 // drilling quality : high 
#define JF_PGRV 0x400000 // Partial Groove Editor 


#define IS_RL_SEP(pjob) ((pjob)->job_flag & JF_RL_SEP ? 1 : 0)
#define IS_PROTECTED_JOB(pjob) ((pjob)->job_flag & JF_PROTECTED ? 1 : 0)
#define IS_EDGING_STARTED(pjob) ((pjob)->job_flag & JF_ON_EDGING ? 1 : 0)
#define IS_FPD_MODE(pjob) ((pjob)->job_flag & JF_FPD ? 1 : 0)

#define IS_RETOUCH_MODE(pjob) ((pjob)->job_flag & JF_RETOUCH ? 1 : 0)
#define IS_HOLE_EDIT_MODE(pjob) ((pjob)->job_flag & JF_HOLE_EDIT ? 1 : 0)
#define IS_DIGITAL_PATTERN_MODE(pjob) ((pjob)->job_flag & JF_DIGITAL_PATTERN ? 1 : 0)
#define IS_FRAME_CHANGE_LENS(pjob) ((pjob)->job_flag & JF_FRAME_CHANGE_LENS ? 1 : 0 )
#define IS_DRILL_RETOUCH_MODE(pjob) ((pjob)->job_flag & JF_DRILL_RETOUCH ? 1 : 0 )

#define SET_RETOUCH_MODE(pjob) do { (pjob)->job_flag |= JF_RETOUCH; } while(0)
#define CLEAR_RETOUCH_MODE(pjob) do { (pjob)->job_flag &= (~JF_RETOUCH); } while(0)

#define IS_JOB_DRILLED(pjob, side) (((side) == FRAME_SIDE_R) ? ( (pjob)->job_flag & JF_R_DRILLED) : ((pjob)->job_flag & JF_L_DRILLED)) 

#define IS_DRILL_ADJ_DATA(pjob) ( (pjob)->job_flag & JF_DRILL_ADJ_DATA ? 1 : 0)
#define IS_DRILL_WRAP_MODE(pjob) ( (pjob)->job_flag & JF_DRILL_WRAP ? 1 : 0)

#define IS_HI_POLISH_MODE(pjob) ( (pjob)->job_flag & JF_HI_POLISH ? 1 : 0)
#define SET_HI_POLISH_MODE(pjob) do { (pjob)->job_flag |= JF_HI_POLISH; } while(0)
#define CLEAR_HI_POLISH_MODE(pjob) do { (pjob)->job_flag &= (~JF_HI_POLISH); } while(0)

#define IS_DRILL_HI_QUALITY(pjob) ( (pjob)->job_flag & JF_DQ_HI ? 1 : 0)
#define IS_PGRV_MODE(pjob) ((pjob)->job_flag & JF_PGRV ? 1 : 0)


	// drill data
	struct list_head drill_list;
	struct list_head drill_retouch_list[MAX_FRAME_SIDE];

	// char extra[ZERO_ARRAY_IDX];
	// EZ ������ extra �ʵ带 �̿��� �߰� �ʵ带 �Ҵ�������,
	// spadework ������ �׳� �� ���Ŀ� �ʵ带 �߰� �ϵ��� ����
	
	// �ʵ� �߰��� ���⿡ 
	
	// �۾��� ���� �ð� ������ 
	unsigned int process_tick[MAX_EDGING_PROCESS_IDX];
	RTC_TIME process_start; // ���� ���� �ð� ����
	RTC_TIME process_end; // ���� �Ϸ� �ð� ����

	// feeling data
	int feeler_data_count;
	int *feeler_data_front[MAX_FRAME_SIDE];
	int *feeler_data_rear[MAX_FRAME_SIDE];

	// FC ��带 ���� ���� ��ǥ �̵� delta �� ����
	float shift_x[MAX_FRAME_SIDE];
	float shift_y[MAX_FRAME_SIDE];

	struct retouch_check_data retouch_check[MAX_FRAME_SIDE];
} JOB_DATA;

int get_frame_alloc_opt(struct frame_raw_data *p_frd);
int alloc_frame_raw_data(int opt, struct frame_raw_data *p_frd);
void free_frame_raw_data(struct frame_raw_data *p_frd);
void copy_frame_raw_data(struct frame_raw_data *target, struct frame_raw_data *src);
int alloc_xy_data(struct xy_data *p_xy, int record_count);
void free_xy_data(struct xy_data *p_xy);
void copy_xy_data(struct xy_data *target, struct xy_data *src);
JOB_DATA * alloc_job_data(void);
void free_job_data(JOB_DATA *pjob);



//
// drill_data.c
//
// header include ������ drill_data.c �� prototype ����δ� ���⼭....
// 
#define ADD_DRILL_DATA_FLAG_DEFAULT_POS 0x01
#define ADD_DRILL_DATA_FLAG_PREV_POS 0x02
#define ADD_DRILL_DATA_FLAG_CLOSE_PREV_POS 0x04
#define ADD_DRILL_DATA_FLAG_ZERO_POS 0x08

struct drill_data_entry * add_drill_data(JOB_DATA *pjob, int hole_type, int flag);
struct drill_data_entry * alloc_drill_entry(void);
struct drill_data_entry * find_selected_drill_entry(struct list_head *);
int get_drill_entry_count(struct list_head *phead, int *pno);
int calc_hole_id(struct list_head *phead, struct drill_data_entry *entry);
void free_drilling_list(struct list_head *phead);
struct drill_data_entry * delete_drill_data_entry(JOB_DATA *pjob, struct drill_data_entry *entry);
void recalc_hole_pos(JOB_DATA *pjob, int side, DRILL_DATA *pdrill);
int set_drilling_data_to_job(JOB_DATA *pjob, struct epc_drill_data *p_drill, int drill_count, int add_flag);
void convert_gui_drill_data_to_comm(struct epc_drill_data *p_drill, DRILL_DATA *p_gui, int side, int opt);
struct drill_data_entry * get_drill_entry(struct list_head *phead, int hole_id);
void clear_drill_data_flag(struct list_head *phead, int clear_mask);
void set_drill_data_flag(struct list_head *phead, int set_mask);
int is_drill_entry_disabled(struct drill_data_entry * entry, int side);
int get_hole_id_of_drill_entry(struct list_head *phead, struct drill_data_entry *entry);
int adjust_drill_data(JOB_DATA *pjob);
void convert_comm_drill_data_to_gui(DRILL_DATA *p_gui, struct epc_drill_data *p_drill);
int compare_drill_list(struct list_head *a, struct list_head *b);
void set_drill_data_opt(struct list_head *phead, int set_mask, int side);
void clear_drill_data_opt(struct list_head *phead, int clear_mask, int side);
int calc_drill_coord_for_dp(JOB_DATA *pjob, int step);
int check_hole_count(JOB_DATA *pjob, int add_count);


#endif
/********** end of file **********/
