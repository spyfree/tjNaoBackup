#include "PreviewController.h"
#include "Tools/Math/Common.h"
#include "Platform/GetSetting.h"
#include <iostream>
#include <fstream>
PreviewController::PreviewController()
{
	loadGainFile();
	resetCoefficient(36, 0.22, 0.04, 9.81);
}
PreviewController::~PreviewController()
{

}
//2 Num_Preview_Frames must be intergral multiple of PG
void PreviewController::resetCoefficient(double PG, double Zc, double dt, double g)
{
	if(gainTable.valid)
		setCoefficientFromTable(Zc,dt);
	else
		setDefaultCoefficient();
}
static const bool USE_MANUAL_MODIFY = false;
void PreviewController::setCoefficientFromTable(double zc_, double dt_)
{
	double Zc;
	double zc_test =zc_;
	if(USE_MANUAL_MODIFY)
		zc_ = 0.1;
	if(gainTable.valid)
	{
		numPreviewFrames = gainTable.nFLen;
		int num = gainTable.nLen;
		int N = gainTable.nFLen;
		if(zc_ < gainTable.ZcTable[0])
		{
			Zc = gainTable.ZcTable[0];
			for(int j = 0; j < 3; ++j)
				K[j] = gainTable.KTable[0][j];
			for(int j = 0; j < N; ++j)
				F[j] = gainTable.FTable[0][j];
		}else if(zc_ >= gainTable.ZcTable[num -1])
		{
			Zc = gainTable.ZcTable[num -1];
			for(int j = 0; j < 3; ++j)
				K[j] = gainTable.KTable[num -1][j];
			for(int j = 0; j < N; ++j)
				F[j] = gainTable.FTable[num -1][j];
		}else{
			int index;
			for(int i = 0; i < num-1; ++i)
			{
				if(gainTable.ZcTable[i] <= zc_ && gainTable.ZcTable[i+1] > zc_){
					index = i; break;
				}
			}
			double factor = (zc_ - gainTable.ZcTable[index]) /(gainTable.ZcTable[index+1] - gainTable.ZcTable[index]);
			Zc = (gainTable.ZcTable[index+1] - gainTable.ZcTable[index])*factor + gainTable.ZcTable[index];
			for(int j = 0; j < 3; ++j)
				K[j] = (gainTable.KTable[index+1][j] - gainTable.KTable[index][j])*factor + gainTable.KTable[index][j];
			for(int j = 0; j < N; ++j)
				F[j] = (gainTable.FTable[index+1][j] - gainTable.FTable[index][j])*factor + gainTable.FTable[index][j];
		}
		double dt = gainTable.dt;
		double g = gainTable.g;
		if(USE_MANUAL_MODIFY)
			Zc = zc_test;
		A.c[0] = Vector3<double>(1, 0, 0);
		A.c[1] = Vector3<double>(dt, 1, 0);
		A.c[2] = Vector3<double>(sqr(dt)/2, dt, 1);
		b = Vector3<double>(dt * dt * dt / 6, dt * dt / 2, dt);
		c = Vector3<double>(1, 0, - Zc / g);
		Q = 1.0;
		R = 1e-6;
	}
		
}
void PreviewController::setDefaultCoefficient()
{
	numPreviewFrames = 80;
	double dt = 0.02;
	double g = 9.81;
	double Zc = 0.2246;
	A.c[0] = Vector3<double>(1, 0, 0);
	A.c[1] = Vector3<double>(dt, 1, 0);
	A.c[2] = Vector3<double>(sqr(dt)/2, dt, 1);
	b = Vector3<double>(dt * dt * dt / 6, dt * dt / 2, dt);
	c = Vector3<double>(1, 0, - Zc / g);
	Q = 1.0;
	R = 1e-6;
// 	// TODO: Load from Files and interpolate for different Zc;
// 	// for Zc = 0.2261, dt = 0.04, g = 9.81;
// 	P.c[0]  = Vector3<double>(9.1374, 1.4871, 0.0172);
// 	P.c[1]  = Vector3<double>(1.4871, 0.2860, 0.0100);
// 	P.c[2]  = Vector3<double>(0.0172, 0.0100, 0.0013);
	//9.81
	 	K = Vector3<double>(506.4678,  185.1113,   22.1544);
	 	double tmp[MAX_PREVIEW_FRAMES] = {-233.767303959717,25.6897839799151,108.628218067317,120.890666697803,107.871106940608,88.3040325940266,69.3562671580074,53.3169903027186,40.5261400694582,30.6286974668470,23.0912348324395,17.3987637611301,13.1170576877775,9.90141115309432,7.48644745059476,5.67118201953095,4.30472142804054,3.27427383489803,2.49567975457860,1.90615886093755,1.45884537597801,1.11870776284879,0.859516150238153,0.661593097809399,0.510146173333164,0.394031234017984,0.304834134139668,0.236187834661800,0.183263662891608,0.142391566210212,0.110776062008135,0.0862833085624295,0.0672811357858792,0.0525185936809231,0.0410350514112045,0.0320914424027469,0.0251181434167313,0.0196753756983424,0.0154230541418267,0.0120977812058851,0.00949525595888569,0.00745679646501625,0.00585899350097967,0.00460575313940491,0.00362216558385243,0.00284977297486507,0.00224291095623336,0.00176587594664550,0.00139072851673754,0.00109558765677267,0.000863304496154474,0.000680429793863004,0.000536409203322109,0.000422955388886042,0.000333557636828158,0.000263098494681828,0.000207553820067617,0.000163757902713390,0.000129219405581648,0.000101977030612214,8.04862637517421e-05,6.35304551004698e-05,5.01509677448467e-05,3.95922789028845e-05,3.12588130981264e-05,2.46809860475626e-05,1.94884837179162e-05,1.53892275574067e-05,1.21528105822904e-05,9.59745022752807e-06,7.57970854161833e-06,5.98639077516856e-06,4.72815930839923e-06,3.73449869254812e-06,2.94974520548050e-06,2.32995532306530e-06,1.84043546748306e-06,1.45379311588570e-06,1.14839903457299e-06,9.07173767296381e-07};
	//2
	//  	K = Vector3<double>(187.8624,  134.3217,   26.7540);
	//  	double tmp[MAX_PREVIEW_FRAMES] = {-159.426942152338, 30.8953648921594, 35.1479277622667, 31.5482679226141, 28.0225149851845, 24.8777957641309, 22.0854505059677, 19.6065469492619, 17.4059207492907, 15.4523293057140, 13.7180371385172, 12.1784231589362, 10.8116309495900, 9.59825827753438, 8.52108147997886, 7.56481080717914, 6.71587324492841, 5.96221972970493, 5.29315401635607, 4.69918076605410, 4.17187069552070, 3.70374087107352, 3.28814844635063, 2.91919633367867, 2.59164946868808, 2.30086047835614, 2.04270369631783, 1.81351658792676, 1.61004775286117, 1.42941076655091, 1.26904320468027, 1.12667026867787, 1.00027249548670, 0.888057092944337, 0.788432493620447, 0.699985765688458, 0.621462560001420, 0.551749308574959, 0.489857421665800, 0.434909259026989, 0.386125676124406, 0.342814968471712, 0.304363057100518, 0.270224775811629, 0.239916136502213, 0.213007462755193, 0.189117294208406, 0.167906975167369, 0.149075850642542, 0.132357001617594, 0.117513459012294, 0.104334842600678, 0.0926343771790491, 0.0822462436345219, 0.0730232273196707, 0.0648346303596087, 0.0575644182647543, 0.0511095745486747, 0.0453786400030721, 0.0402904159031184, 0.0357728127431687, 0.0317618281684688, 0.0282006396021286, 0.0250387986944135, 0.0222315161664490, 0.0197390269032144, 0.0175260262894936, 0.0155611697933729, 0.0138166286993234, 0.0122676956895933, 0.0108924346798962, 0.00967136994323846, 0.00858721011311235, 0.00762460315209381, 0.00676991881115253, 0.00601105549494544, 0.00533726879455454, 0.00473901925646204, 0.00420783722939380, 0.00373620287287360};
	//g = 18
// 	K = Vector3<double>(586.6844 ,178.6170,   19.8109);
// 	double tmp[MAX_PREVIEW_FRAMES] = {-169.498652421966, 12.1280603971484, 99.5706702556450, 129.149892129066, 126.620599353771, 108.913148453556, 86.2876355244888, 64.3459653843345, 45.6704101194184, 31.0402316400393, 20.2687599262202, 12.7383055160681, 7.71446447362729, 4.51085007516609, 2.55964576043603, 1.42777583277157, 0.805284441135373, 0.482410608860348, 0.324701241958253, 0.250799099006651, 0.214671747025233, 0.192454202328549, 0.173313430675621, 0.153475635248739, 0.132559775033465, 0.111487658908937, 0.0914065725718438, 0.0732193980337359, 0.0574496508309522, 0.0442695428728132, 0.0335903828481934, 0.0251617885287508, 0.0186553748516098, 0.0137252527453670, 0.0100461852941994, 0.00733380628526205, 0.00535211101030392, 0.00391289081597757, 0.00287074043456079, 0.00211617367643371, 0.00156844829736706, 0.00116899264281764, 0.000875838731877588, 0.000659160252653021, 0.000497842618837271, 0.000376932601131109, 0.000285792463547005, 0.000216792768038064, 0.000164402130371188, 0.000124560787167302, 9.42520995319297e-05, 7.12093888126279e-05, 5.37139690281871e-05, 4.04541294435621e-05, 3.04248211484709e-05, 2.28547411589803e-05, 1.71521706584380e-05, 1.28639704767765e-05, 9.64408084793030e-06, 7.22909153050713e-06, 5.41920668741671e-06, 4.06340331918289e-06, 3.04788606085568e-06, 2.28714474072568e-06, 1.71706562513980e-06, 1.28965621020188e-06, 9.69029591672458e-07, 7.28364678729203e-07, 5.47616485055173e-07, 4.11798563546866e-07, 3.09698802341275e-07, 2.32921455186350e-07, 1.75173518647993e-07, 1.31733409052551e-07, 9.90552786822770e-08, 7.44740968064418e-08, 5.59855515829567e-08, 4.20815336686563e-08, 3.16269591065693e-08, 2.37673922591778e-08};
	for(int i = 0; i< MAX_PREVIEW_FRAMES; ++i)
	{
		F[i] = tmp[i];
	}
}
bool PreviewController::loadGainFile()
{
	std::string filename = std::string(GetSetting::GetConfigDir()) + "/WalkCfg/PreGainTable.dat";
	return gainTable.load(filename);
}

void PreviewController::resetController(const Vector3<double>& stateX0, const Vector3<double>& stateY0)
{
	VecX = stateX0;
	VecY = stateY0;
}
Vector3<double> PreviewController::getRefCOM(PreviewBuffer& preBuffer, const PreviewWalkParam& walkParam)
{
	Vector3<double> RefCOM;
	Vector3<double> com;
	double uxk= 0, uyk = 0;
	double sumx1  = 0,sumx2 = 0,  sumy1 = 0, sumy2 = 0;
	int i = 0, j = 0;
	const PreviewState& stateV = preBuffer.front();
	
	// Should use k+1 : k + N, then the buffer size should be N +1
	for(i = 1; i < preBuffer.size(); ++i)
	{
		sumx1 += F[i-1] * preBuffer[i].pZMP.x;
		sumy1 += F[i-1] * preBuffer[i].pZMP.y;
// 		if(stateV.supmod == NaoConfig::SUPPORT_MODE_LEFT && stateV.istep == 0)
// 			std::cout<<preBuffer[i].pZMP.y<<std::endl;
	}
	uxk = - (K * VecX) + sumx1;
	uyk = - (K * VecY) + sumy1;

	VecX = A * VecX + b * uxk;
	VecY = A * VecY + b*uyk;

	com.x = VecX[0] - stateV.pSupFootZMP.x;
	com.y = VecY[0] - stateV.pSupFootZMP.y;
	com.z = stateV.COMz;

	com = RotationMatrix::getRotationZ(-stateV.rSupFootZMP.z) * com;
	if(stateV.supmod == NaoConfig::SUPPORT_MODE_LEFT || stateV.supmod == NaoConfig::SUPPORT_MODE_DOUBLE_LEFT)
		com = com + Vector3<double>(walkParam.ZmpOffsetLeft.x, walkParam.ZmpOffsetLeft.y, 0);
	else
		com = com + Vector3<double>(walkParam.ZmpOffsetRight.x, walkParam.ZmpOffsetRight.y, 0);

	RefCOM = com;
	return RefCOM;
}

PreviewController::GainTable::GainTable()
:valid(false), nLen(0), nFLen(0)
{
}
bool PreviewController::GainTable::load(const std::string& filename)
{
	bool readOK = true;
	bool finish = false;
	std::ifstream fin(filename.c_str());
	std::cout<<"Loading Preview Gain File...       ";
	if(!fin){
		readOK = false;
	}else{
		//ignore first lines beginning with '#'
		while(fin.peek() == '#' || isspace(fin.peek())){
			if(fin.peek() == '#')
				while(fin.peek() != '\n')
					fin.get();
			else if(isspace(fin.peek()))
				while(isspace(fin.peek()))
					fin.get();
		}
		fin >> nLen >> nFLen>> dt >> g;
		if(nLen < 0) nLen = 0;else if(nLen > MAX_TABLE_NUM) nLen = MAX_TABLE_NUM;
		if(nFLen < 0) nFLen = 0; else if(nFLen > MAX_TABLE_PREVIEW_FRAMES) nFLen = MAX_TABLE_PREVIEW_FRAMES;
		for(int i = 0; i < nLen; ++i)
		{
			//ignore first lines beginning with '#'
			while(fin.peek() == '#'){
				while(fin.peek() != '\n')
					fin.get();
			}
			fin >> ZcTable[i];
			for(int j = 0; j < 3; ++j)
				fin >> KTable[i][j];
			for(int k = 0; k <nFLen; ++k )
				fin >> FTable[i][k];
			if(!fin.good())
			{
				std::cout<<"Error in Preview "<< filename.c_str()<< " - unexpected EOF" << std::endl;
				readOK = false;
				break;
			}
		}
	}
	//======
	if(readOK){
		std::cout<<"Done!"<<std::endl;
	}
	else{
		std::cout<<"Failed! File: "<<filename.c_str()<<std::endl;
	}
	fin.close();
	if(readOK)
		valid = true;
	else
		valid = false;
	return readOK;
}