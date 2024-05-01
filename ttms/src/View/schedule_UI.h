   #ifndef SCHEDULE_UI_H_
   #define SCHEDULE_UI_H_
   
   
    #include "../Common/List.h"
	#include "../Service/schedule_Ser.h"
	#include "../Service/Play_Ser.h"
	
	void Schedule_UI_MgtEnt();
	int Schedule_UI_Add(int play_id);
	int Schedule_UI_Modify(int id);
	int Schedule_UI_Delete(int id);
	
	
	#endif /* SCHEDULE_UI_H_ */
