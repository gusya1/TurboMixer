
enum class ButtonState
{
    Released = 0,
    Pressed,
};

class IButtonWatcher
{
public:
    virtual void setup() = 0;
    virtual bool clicked() const = 0;
    virtual bool longPressed() const = 0;
    virtual ButtonState state() const = 0;
};
