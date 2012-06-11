
class Controller
{
	public:
		Controller();
		virtual ~Controller();
		virtual void handle()=0;
	
	protected:
		int m_id;
		int get_id();

};
