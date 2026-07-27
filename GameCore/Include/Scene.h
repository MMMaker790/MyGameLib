class Scene
{
public:
    Scene(class Game *game)
        : mGame(game), mStatus(Status::EInactive) {
          };
    ~Scene();

    // 入退場
    void OnEnter();
    void OnExit();

    // 更新
    // 値の追加を考慮して構造体で渡す
    struct UpdateContext
    {
        float deltaTime;
    };
    void Update(const UpdateContext &context);

    void Draw();

    // 状態
    enum class Status
    {
        EActive,
        EInactive,
        EExiting,
        EUnloading,
    };
    Status GetStatus() const { return mStatus; }
    void SetStatus(Status status) { mStatus = status; }

private:
    class Game *mGame;
    Status mStatus;
};