
enum Role
{
    NullRole,
    DUKE,
    ASSASSIN,
    AMBASSADOR,
    CAPTAIN,
    CONTESSA,
};

enum Action
{
    NullAction,
    INCOME,
    FOREIGN_AID,
    COUP,
    TAX,
    ASSASSINATE,
    EXCHANGE,
    STEAL,
    BLOCK_FOREIGN_AID,
    BLOCK_ASSASSINATE,
    BLOCK_STEAL_CAPTAIN,
    BLOCK_STEAL_AMBASSADOR,
    CHALLENGE,
};

enum ActionStatus
{
    NullStatus,
    VALID,
    BLOCKED,
    CHALLENGED,
};

