namespace yubi
{
    class WorkArea
    {
    public:
        WorkArea() = default;
        WorkArea(WorkArea &&) = default;
        WorkArea(const WorkArea &) = default;
        WorkArea &operator=(WorkArea &&) = default;
        WorkArea &operator=(const WorkArea &) = default;
        ~WorkArea() = default;

    private:
    };
}
