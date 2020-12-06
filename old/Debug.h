#include <mymap.h>

/*
typedef struct _CrtMemBlockHeader
{
  struct _CrtMemBlockHeader * pBlockHeaderNext;
  struct _CrtMemBlockHeader * pBlockHeaderPrev;
  char* szFileName;
  int  nLine;
  size_t nDataSize;
  int nBlackUse;
  long lRequest;
  unsigned char gap[nNoMansLandSize];  
  unsigned char data[nDataSize];
  unsigned char anotherGap[nNoMansLandSize];
} _CrtMemBlockHeader;
*/

class TTime_Logger
{
	public:
		int array[20];
		int total_count;
		int last;
		TTime_Logger()
		{
			total_count = 20;
			//array = new int[total_count];
			for (int i = 0; i != total_count; ++i) array[i] = 0;
			last = 0;
		};
		
		~TTime_Logger()
		{
			//delete[] array;	
		};
		
		void RegisterTime()
		{
			array[last]=CurTime();
			last+=1;
			if (last >= total_count) last = 0;
		};
		
		float GetFrequency()
		{
			int last_reg = array[last];
			float freq = (float)(CurTime()-last_reg)/total_count/1000;
			return 0;
		};
};

size_t memory_total_allocated = 0;
size_t texture_total_allocated = 0;
class TMemoryRecord
{
	public:
		int times_recorded;
		int times_used;
		size_t memory_total;
		map<void*,size_t> pointers;
		
		void Alloc(void *ptr, size_t size)
		{
			times_used+=1;
			times_recorded+=1;
			memory_total+=size;
			memory_total_allocated += size;
			pointers[ptr]=size;
		};
		void Delete(map<void*,size_t>::iterator& it)
		{
			times_recorded-=1;
			memory_total-=it->second;
			memory_total_allocated-=it->second;
			pointers.erase(it);
		};
		inline map<void*,size_t>::iterator find(void* ptr)
		{
			return pointers.find(ptr);
		};
};

class TDebug
{
	public:
		int texture_allocation_total;
		TTime_Logger texture_allocation_time;

		int memory_allocation_total;
		TTime_Logger memory_allocation_time;

		int neurons_total;
		bool showinfo;
		bool logging;
		
		map<TFileLine,TMemoryRecord> memory_records;
		
		TDebug()
		{
			cout << "INITIALIZING TDebug!" << endl;
			texture_allocation_total = 0;
			memory_allocation_total = 0;
			neurons_total = 0;
			showinfo = 0;
			logging = 0;
		};
		
		~TDebug()
		{
			TDebug_initialized = 0;
		};
		
		void Draw()
		{
			if (!showinfo) return;
			Vec2i window_pos = Vec2i(0,22);
			Vec2i window_size = Vec2i(200,200);
			int left_padding = 10;
			int top_padding = 10;
			int line_padding = 10;
			int font_height = MainFont_Height;
			line_padding+=font_height;
			char buf1[100];
			char buf2[100];
			Vec2i textpos = Vec2i(left_padding,top_padding+font_height);
			
			glPushMatrix();
			glTranslatef(window_pos.x,window_pos.y,0);
			
				glColor4f(0.1,0.1,0.1,0.3);
				drawrect(0,window_size);
				
				MemToChar(memory_total_allocated,buf1,100);
				snprintf(buf2,100,"Total memory: %s",buf1);
				
				glColor4f(1,1,1,0.6);
				drawtext(textpos.x, textpos.y, buf2); textpos.y += line_padding;
				drawtext(textpos.x, textpos.y, "Most large memory allocators: "); textpos.y += line_padding;
				
				map<size_t,TFileLine> sorted_array;
				for (auto it = memory_records.begin(); it != memory_records.end(); ++it)
					sorted_array[it->second.memory_total] = it->first;
				
				int i = 0;
				const char*	ptr;
				const char*	last;
				for (auto it = sorted_array.rbegin(); it != sorted_array.rend(); ++it)
				{
					++i;
					ptr = it->second.file; last=ptr;
					if (ptr) while (*ptr) if (*ptr == '/') last = ++ptr; else ++ptr;
					MemToChar(it->first,buf1,100);
					snprintf(buf2,100,"%s:%d:	%s",last,it->second.line,buf1);
					drawtext(textpos.x, textpos.y, buf2); textpos.y += line_padding;
					if (i == 5) break;
				};
			glPopMatrix();
		};
		
		void AddNeuron()
		{
			++neurons_total;
		};
		
		void DeleteNeuron()
		{
			--neurons_total;
		};
		
		void AllocTexture()
		{
			if (logging) Console << "Texture allocated" << endl;
			++texture_allocation_total;
			texture_allocation_time.RegisterTime();
		};
		
		void DeallocTexture()
		{
			if (logging) Console << "Texture deleted" << endl;
			--texture_allocation_total;
		};
		
		void AllocMem(void* ptr, const char* file, size_t line, size_t size)
		{
			if (TDebug_initialized != 1234567890) return;
			TDebug_initialized = 0;
			TFileLine fline(file,line);
			memory_records[fline].Alloc(ptr,size);
			TDebug_initialized = 1234567890;
		};
			
		void DeallocMem(void* ptr)
		{	
			if (TDebug_initialized != 1234567890) return;
			TDebug_initialized = 0;
			for (auto it = memory_records.begin(); it != memory_records.end();++it)
			{
				auto ptrit = it->second.find(ptr);
				if (ptrit != it->second.pointers.end()) 
				{
					//cout << "deleting: " << it->first.file << ":" << it->first.line << endl;
					it->second.Delete(ptrit);
				};
			};
			TDebug_initialized = 1234567890;
		};
		
		void ShowInfo(bool ib)
		{
			showinfo = ib;
		};
		
		void DumpAllocatedMemory()
		{
			
			char buf[50];
			buf[0] = 0;
			MemToChar(memory_total_allocated,buf,50);
			Console << "######Total memory: " << buf << endl;
			const char* last;
			
			const char* ptr;

			for (auto it = memory_records.begin(); it != memory_records.end(); ++it)
			{
				if (it->second.memory_total != 0)
				{
					ptr = it->first.file;
					last=ptr;
					if (ptr) while (*ptr) if (*ptr == '/') last = ++ptr; else ++ptr;
					MemToChar(it->second.memory_total,buf,50);
					Console << last << ":" << it->first.line 
					<< " - (" << buf << "/" << it->second.times_recorded << ")" << endl;
				};
			};
			
		};
};

TDebug Debug;

const char* __file__ = "unknown";
size_t __line__ = 0;

const char* __file2__= 0;
size_t __line2__= 0;

#ifdef Debugging

void* operator new(size_t size) {
    void *ptr = malloc(size);
    if (TDebug_initialized == 1234567890)
	{
		if (__file2__ != 0)
		{
			Debug.AllocMem(ptr,__file2__,__line2__,size);
			__file2__ = 0;
			__line2__ = 0;
		}
		else
		{
			Debug.AllocMem(ptr,__file__,__line__,size);
		};
		__file__ = "unknown";
    	__line__ = 0;
	};
    return ptr;
};

void operator delete(void *ptr)
{
   Debug.DeallocMem(ptr);
   free(ptr);
};

#endif

#ifdef Debugging 
	#define glGenTextures(a, b) {Debug.AllocTexture(); glGenTextures(a, b);}
	#define glDeleteTextures(a, b) {Debug.DeallocTexture(); glDeleteTextures(a, b);}
	
	#define new2 (__file__=__FILE__,__line__=__LINE__) && 0 ? NULL : new
	#define new new2
#else
//	#define glGenTextures(a, b) glGenTextures(a, b);
//	#define glDeleteTextures(a, b) glDeleteTextures(a, b);
#endif

void debug_showinfo(bool ib)
{
	Debug.ShowInfo(ib);
};

void debug_info_on()
{
	Debug.ShowInfo(true);
};

void debug_info_off()
{
	Debug.ShowInfo(false);
};

void debug_dumpmem()
{
	Debug.DumpAllocatedMemory();
};
ConsoleRegisterFunction( debug_showinfo, "show info on the screen" );
ConsoleRegisterFunction( debug_dumpmem, "memory that is currently allocated" );

ConsoleRegisterFunctionCustomName( debug_info_on, "+debug_info", 0 );
ConsoleRegisterFunctionCustomName( debug_info_off, "-debug_info", 0 );
