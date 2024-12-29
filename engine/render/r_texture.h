// SimpleGraphic Engine
// (c) David Gowor, 2014
//
// Render Texture Header
//

// =======
// Classes
// =======

#include <atomic>
#include <memory>
#include <string>

class image_c;
class mip_set_c;

// Texture
class r_tex_c {
public:
	int		error;
	enum Status
	{
		INIT,
		IN_QUEUE,
		PROCESSING,
		SIZE_KNOWN,
		PENDING_UPLOAD,
		DONE,
	};
	std::atomic<Status> status;
	std::atomic<int> loadPri;
	dword	texId;
	int		flags;
	std::string fileName;
	std::atomic<dword> fileWidth;
	std::atomic<dword> fileHeight;
	std::shared_ptr<mip_set_c> mipSet;

	r_tex_c(class r_ITexManager* manager, const char* fileName, int flags);
	r_tex_c(class r_ITexManager* manager, std::unique_ptr<image_c> img, int flags);
	~r_tex_c();

	void	Bind();
	void	Unbind();
	void	Enable();
	void	Disable();
	void	StartLoad();
	void	AbortLoad();
	void	ForceLoad();
	void	LoadFile();

	static void PerformUpload(r_tex_c*);

	static int GLTypeForImgType(int type);
private:
	class t_manager_c* manager;
	class r_renderer_c* renderer;
	void	Init(class r_ITexManager* manager, const char* fileName, int flags);
	void	Upload(mip_set_c& mipSet, int flags);
	std::shared_ptr<mip_set_c> BuildMipSet(std::unique_ptr<image_c> img);
};

// ==========
// Interfaces
// ==========

// Texture Manager
class r_ITexManager {
public:
	static r_ITexManager* GetHandle(r_renderer_c* renderer);
	static void FreeHandle(r_ITexManager* hnd);

	virtual int		GetAsyncCount() = 0;
	virtual void	ProcessPendingTextureUploads() = 0;
};
