#include "ModuleGeneric.h"
#include "main.h"
#include "CTRInterface.h"
#include "LCM12864.h"
#include "SaveData.h"


static void Init(){
	SaveData_Read();
	
	// �ر�LCD����
	LCM12864_Backlight(1);
	
	LCM12864_Refresh();
}

void ModuleGeneric(){
	Init();
	
	CTRInterface_Prepare();
	while(1){
		CTRInterface_Update();
	}
}
