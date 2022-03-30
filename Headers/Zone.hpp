namespace yubi
{
    class Zone
    {
    public:
        Zone() = default;
        Zone(Zone &&) noexcept = default;
        Zone(const Zone &) = default;
        Zone &operator=(Zone &&) noexcept = default;
        Zone &operator=(const Zone &) = default;
        ~Zone() = default;

    private:
    };
}
