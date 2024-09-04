#ifndef _BPM_HPP_
#define _BPM_HPP_

class BPM
{
public:
	BPM() {};
	~BPM();
	BPM(float value, float offset);

    void update(int ptime);
	float getValue();
	int getBeatcount();
	bool getChanged();


private:
	float value;
	float offset;
	int framecount;
	int previousbeatdist;
	bool changed;
    int time;


};


#endif
