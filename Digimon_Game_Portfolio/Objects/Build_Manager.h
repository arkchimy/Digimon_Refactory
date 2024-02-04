#pragma once

class Build_Actor 
{
public:
	Build_Actor();
	~Build_Actor();

	void Animation() ;

	void Update() ;
	void Render() ;
	void ViewProjection(D3DXMATRIX& V, D3DXMATRIX& P) ;
	void UpdateWorld() ;

	vector<unique_ptr<class Sprite>> sprites;
};

