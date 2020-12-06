#include<sstream>

#include <TConsole.h>

#include <Debug.h>

#include <GUI/Base/TUI_ParentLess.h>

#include <GUI/Base/TUI_SimpleFrame.h>


#include <GUI/Base/TUI_Frame.h>
#include <GUI/Elements/TUI_Label.h>
#include <GUI/Elements/TUI_Bitmap.h>

#include <GUI/Elements/TUI_Scrollbar.h>
#include <GUI/Elements/TUI_Scrollwheel.h>
#include <GUI/Elements/TUI_ScrollFrame.h>
#include <GUI/Elements/TUI_Button.h>
#include <GUI/Elements/TUI_BitmapButton.h>
#include <GUI/Elements/TUI_Menu.h>
#include <GUI/Elements/TUI_ContextButton.h>

#include <GUI/Elements/TUI_Edit.h>
#include <GUI/Elements/TUI_Memo.h>

#include <GUI/Windows/TUI_FPSMark.h>

#include <GUI/Elements/TUI_MenuButton.h>

#include <GUI/Elements/TUI_MenuPanel.h>

#include <GUI/Elements/TUI_Slider.h>
#include <GUI/Elements/TUI_ValueBox.h>
#include <GUI/Elements/TUI_ParameterBox.h>

#include <GUI/Windows/TWF_Window.h>

//############################# DELAYED PAINTING ##############################
#include <TDelayedPaint.h>

//############################# INCLUDING ENTITIES ############################
#include <Entities/TBaseSingleCell.h>



#include <NNet/Neurons.h>
#include <NNet/NNetwork.h>
#include <GUI/Windows/TWF_NRender.h>

#include <Entities/TFood.h>
#include <Entities/TSeed.h>
#include <Entities/TRock.h>
#include <Entities/TPoison.h>

#include <Entities/TBot.h>

#include <Instancing.h>
#include <Entities/TSpawner.h>
#include <Instancing.cpp>

#include <GUI/Windows/PaintMenu/TWF_PaintMenu.h>
#include <GUI/Windows/TWF_GraphView.h>
#include <GUI/Windows/TWF_Console.h>
#include <GUI/Windows/TWF_Properties.h>

