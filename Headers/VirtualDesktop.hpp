namespace yubi
{
    class VirtualDesktop
    {
    public:
        VirtualDesktop();
        VirtualDesktop(VirtualDesktop &&) = default;
        VirtualDesktop(const VirtualDesktop &) = default;
        VirtualDesktop &operator=(VirtualDesktop &&) = default;
        VirtualDesktop &operator=(const VirtualDesktop &) = default;
        ~VirtualDesktop();

    private:
        
    };

    VirtualDesktop::VirtualDesktop()
    {
    }

    VirtualDesktop::~VirtualDesktop()
    {
    }
}
