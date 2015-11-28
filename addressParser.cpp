// IPCity.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "string.h"
#include "stdio.h"
#include "iostream"
#include "string.h"
#include "vector"
using namespace std;

typedef struct
{
	string stra;	// 行政编码 	        
	string strb;    // 区域名称     
}AREACODE;


typedef struct
{
	string desc;	    //位置详情		        
	string province; //省
	string city;     //市
	string county;   //区/县
}AREADESC;

typedef struct
{
	string code;	    //代码		        
	string name;     //名称
}PRODATA;



vector<AREACODE*> codelist;
vector<AREADESC*> desclist;
vector<PRODATA*> prolist;
vector<PRODATA*> citylist; 
vector<PRODATA*> countylist;

int clearProdata(vector<PRODATA*>& prolist)
{
	vector<PRODATA*>::iterator vTtpl;
	for (vTtpl=prolist.begin();vTtpl!=prolist.end();vTtpl++)
    {
		delete *vTtpl;
		*vTtpl=NULL;
	}
	prolist.clear();
	return 0;
}


int printProdata(vector<PRODATA*>& prolist)
{
	vector<PRODATA*>::iterator vTtpl;
    PRODATA* pl;
	for (vTtpl=prolist.begin();vTtpl!=prolist.end();vTtpl++)
    {
		pl=*vTtpl;
		printf("code=%s,name=%s\n",pl->code.c_str(),pl->name.c_str());
	}
	return 0;
}

int getChildInfo(string code,vector<PRODATA*> &prolist)
{
	AREACODE* pc; 
	vector<AREACODE*>::iterator vItac;
	for ( vItac = codelist.begin(); vItac != codelist.end(); vItac++ )
	{
		pc=*vItac;
        //printf("%s:%s\n",pc->stra.c_str(),pc->strb.c_str());
		if(strcmp(code.c_str(),"NULL")==0)
		{
			if( strcmp(pc->stra.c_str()+2,"0000")==0){
                //printf("%s:%s\n",pc->stra.c_str(),pc->strb.c_str());
				PRODATA *prodata=new PRODATA();
				prodata->code=pc->stra;
				prodata->name=pc->strb;
				prolist.push_back(prodata);			
			} 
		}
		
		if( strcmp(code.c_str()+2,"0000")==0)
		{
			if (strncmp(code.c_str(),pc->stra.c_str(),2)==0 && strcmp(pc->stra.c_str()+4,"00")==0 
				&& strcmp(pc->stra.c_str()+2,"0000")!=0 )
			{
				PRODATA* citycode=new PRODATA();
				citycode->code=pc->stra;
				citycode->name=pc->strb;
				prolist.push_back(citycode);
			}
		}
		
		if( strcmp(code.c_str()+4,"00")==0)
		{
			if (strncmp(code.c_str(),pc->stra.c_str(),4)==0 && strcmp(pc->stra.c_str()+4,"00")!=0)
			{
				PRODATA* countycode = new PRODATA();
				countycode->code=pc->stra;
				countycode->name=pc->strb;
				prolist.push_back(countycode);
			}
		}		
	}
	return 0;
}

int main(int argc, char* argv[])
{

	
	vector< AREACODE* >::iterator vItDisp;
	vItDisp = codelist.begin();
	for (; vItDisp != codelist.end() ; vItDisp++ )
	{
		delete(*vItDisp);
		*vItDisp = NULL;
	}
	codelist.clear();



	FILE* fp=fopen("C:\\areacode.txt","r");
    if (fp == NULL)
	{
        printf("Open areacode.txt error!\n");
		return 1; 
	}
	char aclist[1024]={0};
    while( fgets (aclist,1024, fp)!= NULL )		
	{
        
		aclist[strlen(aclist)-1]='\0';
		AREACODE* areacode = new AREACODE();
		areacode->stra=strtok(aclist,",");
		areacode->strb=strtok(NULL,",");
		codelist.push_back(areacode);
	}
	fclose(fp);
    printf("areacode.txt加载完毕\n");

	fp=fopen("C:\\areadesc.txt","r");
    if (fp == NULL)
	{
        printf("Open areadesc.txt error!\n");
		return 1; 
	}

    char adlist[1024]={0};
    while( fgets (adlist,1024, fp)!= NULL )		
	{
		adlist[strlen(adlist)-1]='\0';
		AREADESC* areadesc = new AREADESC();
		areadesc->desc=adlist;
		desclist.push_back(areadesc);
	}
    fclose(fp);
	printf("areadesc.txt加载完毕\n");

	clearProdata(prolist);
    getChildInfo("NULL",prolist);

	vector<PRODATA*>::iterator vTtpl;
    PRODATA* pl=NULL;
	vector<PRODATA*>::iterator vTtcl;
    PRODATA* cl=NULL;
	vector<PRODATA*>::iterator vTtxl;
    PRODATA* xl=NULL;

	AREADESC* dc=NULL; 
	vector<AREADESC*>::iterator vItdc;
	for ( vItdc = desclist.begin(); vItdc != desclist.end(); vItdc++ )
	{
		dc=*vItdc;
		//printf("地址:%s\n",dc->desc.c_str());
        for (vTtpl=prolist.begin();vTtpl!=prolist.end();vTtpl++)
        {
			pl=*vTtpl;
			if(strstr(dc->desc.c_str(),pl->name.c_str())!=NULL)
			{
				dc->province=pl->code;
				//printf("省代码 = %s\n",dc->province.c_str());
				if (strcmp(dc->desc.c_str(),pl->name.c_str())==0)
				break;
				clearProdata(citylist);
                getChildInfo(pl->code,citylist);
				for (vTtcl=citylist.begin();vTtcl!=citylist.end();vTtcl++)
				{
					cl=*vTtcl;
					if (strstr(dc->desc.c_str(),cl->name.c_str())!=NULL)
					{
						dc->city=cl->code;
                        //printf("市代码 = %s\n",dc->city.c_str());
                        clearProdata(countylist);
						getChildInfo(cl->code,countylist);
                        for (vTtxl=countylist.begin();vTtxl!=countylist.end();vTtxl++)
                        {
							xl=*vTtxl;
							if (strstr(dc->desc.c_str(),xl->name.c_str())!=NULL)
							{
								dc->county=xl->code;
							//	printf("县代码 = %s\n",dc->county.c_str());
								break;
							}
							
                        }
						break;

					}
					
				}
				break;

			}
        }
                 

	}
    printf("行政区划解析完毕\n");

	fp=fopen("C:\\descresult.csv","w");
	for ( vItdc = desclist.begin(); vItdc != desclist.end(); vItdc++ )
	{
		dc=*vItdc;
		fprintf(fp,"%s,%s,%s,%s\n",dc->desc.c_str(),dc->province.c_str(),dc->city.c_str(),dc->county.c_str());
	}
	fclose(fp);

        
	return 0;
}











