
enum class ButtonState
{
    Released = 0,
    Pressed,
};

class IButtonWatcher
{
public:
    virtual void setup() = 0;
    virtual bool clicked() = 0;
    virtual bool longPressed() = 0;
    virtual ButtonState state() = 0;
};
