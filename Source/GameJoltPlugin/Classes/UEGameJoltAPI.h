// Copyright (c) 2019 Free2Play-Entertainment

#pragma once
#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Dom/JsonValue.h"
#include "UEGameJoltAPI.generated.h"

/* Generates a delegate for the OnGetResult event */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGetResult);

/* Generates a delegate for the OnFailed event */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFailed);

/* Represents all possible requests */
UENUM(BlueprintType)
enum class EGameJoltComponentEnum : uint8
{
	GJ_USER_AUTH		UMETA(DisplayName = "Authorize User"),
	GJ_USER_FETCH		UMETA(DisplayName = "Fetch User"),
	GJ_SESSION_OPEN	    UMETA(DisplayName = "Open Session"),
	GJ_SESSION_PING 	UMETA(DisplayName = "Ping Session"),
	GJ_SESSION_CLOSE 	UMETA(DisplayName = "Close Session"),
	GJ_TROPHIES_FETCH 	UMETA(DisplayName = "Fetch Trophies"),
	GJ_TROPHIES_ADD 	UMETA(DisplayName = "Add Achieved Trophies"),
	GJ_SCORES_FETCH 	UMETA(DisplayName = "Fetch Score"),
	GJ_SCORES_ADD 		UMETA(DisplayName = "Add Score"),
	GJ_SCORES_TABLE 	UMETA(DisplayName = "Get Score Tables"),
	GJ_DATASTORE_FETCH	UMETA(DisplayName = "Fetch Data Store"),
	GJ_DATASTORE_SET	UMETA(DisplayName = "Set Data Store"),
	GJ_DATASTORE_UPDATE	UMETA(DisplayName = "Update Data Store"),
	GJ_DATASTORE_REMOVE UMETA(DisplayName = "Get Keys"),
	GJ_OTHER			UMETA(DisplayName = "Other"),
	GJ_TIME				UMETA(DisplayName = "Fetch Server Time")
};

/* Represents the possible selections for "Fetch Trophies" (all, achieved, unachieved) */
UENUM(BlueprintType)
enum class EGameJoltAchievedTrophies : uint8
{
	GJ_ACHIEVEDTROPHY_BLANK UMETA(DisplayName = "All Trophies"),
	GJ_ACHIEVEDTROPHY_USER UMETA(DisplayName = "User Achieved Trophies"),
	GJ_ACHIEVEDTROPHY_GAME UMETA(DisplayName = "Unachieved Trophies")
};

/* Contains all available information about a user */
USTRUCT(BlueprintType)
struct FUserInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User ID")
		int32 S_User_ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User type")
		FString User_Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Username")
		FString User_Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Avatar")
		FString User_AvatarURL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Signed up")
		FString Signed_up;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Last Logged in")
		FString Last_Logged_in;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Status")
		FString status;
};

/* Contains all information about a trophy */
USTRUCT(BlueprintType)
struct FTrophyInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trophy ID")
	int32 Trophy_ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trophy's Name")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trophy's Description")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trophy's Difficulty")
		FString Difficulty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trophy's Image URL")
		FString image_url;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achieved Time")
		FString achieved;
};

/* Contains all information about an entry in a scoreboard */
USTRUCT(BlueprintType)
struct FScoreInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score String")
	FString ScoreString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Sort")
		int32 ScoreSort;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Extra Data")
		FString extra_data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Username")
		FString UserName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score User ID")
		int32 User_Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Guest Username")
		FString guestUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Stored")
		FString stored;

};

/* Contains all information about a scoreboard */
USTRUCT(BlueprintType)
struct FScoreTableInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard Table ID")
		int32 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard Table Name")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard Table Description")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scoreboard Table Primary")
		FString Primary;

};

/**
 * Class to use the GameJoltAPI
 * Is also internally used by an UUEGameJoltAPI instance as a carrier for response data
*/
UCLASS(BlueprintType, Blueprintable)
class GAMEJOLTPLUGIN_API UUEGameJoltAPI : public UObject
{
	GENERATED_UCLASS_BODY()

private:

	/**
	* Callback for IHttpRequest::OnProcessRequestComplete()
	* @param Request HTTP request pointer
	* @param Response Response pointer
	* @param bWasSuccessful Whether the request was successful or not
	*/
	void OnReady(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	/* Reset Data*/
	void Reset();

	/**
	 * Creates a http-URL from the input
	 * @param inputURL The string to create the URL from
	 * @return An URL starting with http://
	*/
	static FString CreateURL(FString inputURL);

	void WriteObject(TSharedRef<TJsonWriter<TCHAR>> writer, FString key, FJsonValue* value);

public:

	UObject* contextObject;

	/* Prevents crashes in Get-Functions */
	UPROPERTY(Transient)
	class UWorld* World;

	/* Allows usage of the World-Property */
	virtual class UWorld* GetWorld() const override;

	/* Whether the user uses a guest profile */
	UPROPERTY(BlueprintReadOnly, Category = "GameJolt|User")
	bool bGuest;

	/* The username of the guest profile */
	UPROPERTY(BlueprintReadWrite, Category = "GameJolt|User")
	FString Guest_username;

	/* Whether the user is logged in or not */
	UPROPERTY(BlueprintReadOnly, Category = "GameJolt|User")
	bool m_LoggedIn;

	/* Will be removed in 1.7 */
	UPROPERTY(meta = (DeprecatedProperty, DeprecationMessage="Use LastActionPerformed instead"))
	EGameJoltComponentEnum GameJoltComponentEnum_DEPRECATED;

	/* An enum representing the last request send. Local 'Get' nodes don't count */
	UPROPERTY(BlueprintReadWrite, Category = "GameJolt")
	EGameJoltComponentEnum LastActionPerformed;

	/* The actual field data */
	TSharedPtr<FJsonObject> Data;

	/* Contains the actual page content, as a string */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameJolt|Request")
	FString Content;

	/* Event which triggers when the content has been retrieved */
	UPROPERTY(BlueprintAssignable, Category = "GameJolt")
	FOnGetResult OnGetResult;

	/* Event which triggers when the request failed */
	UPROPERTY(BlueprintAssignable, Category = "GameJolt")
	FOnFailed OnFailed;

	/* Creates new data from the input string */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "From String"), Category = "GameJolt|Request")
	void FromString(const FString& dataString);

	/* Creates a new post data object */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Create GameJolt API Data", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "GameJolt")
	static UUEGameJoltAPI* Create(UObject* WorldContextObject);

	/* GameID */
	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "Your Game ID"), Category = "GameJolt")
	int32 Game_ID;

	/* Private Key */
	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "Your Game Private Key"), Category = "GameJolt")
	FString Game_PrivateKey;

	/* Username */
	UPROPERTY(BlueprintReadOnly, meta = (DisplayName = "Players Username"), Category = "GameJolt|User")
	FString UserName;

private:
	/* Token */
	UPROPERTY()
	FString UserToken;

public:

	/* Properties for HTTP-Request*/
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "GameJolt API Server"), Category = "GameJolt|Request")
	FString GJAPI_SERVER;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "GameJolt API Root"), Category = "GameJolt|Request")
	FString GJAPI_ROOT;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "GameJolt API Version"), Category = "GameJolt|Request")
	FString GJAPI_VERSION;
	/* End of Properties */

	/* Public Functions */

	/* Gets GameID */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Your Game ID"), Category = "GameJolt")
	int32 GetGameID();

	/* Gets Private Key */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Your Game Private Key"), Category = "GameJolt")
	FString GetGamePrivateKey();
	
	/* Gets Username */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Username"), Category = "GameJolt|User")
	FString GetUsername();

	/* Initializes GameJolt API */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Init"), Category = "GameJolt")
	void Init(FString PrivateKey, int32 GameID);

#pragma region Session

	/* Opens a session. You have to ping it manually with a timer. */
	UFUNCTION(BlueprintCallable, meta = (DislayName = "Open Session"), Category = "GameJolt|Sessions")
	bool OpenSession();

	/* Ping the session. Ping it every 30 to 60 seconds. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Ping Session"), Category = "GameJolt|Sessions")
	bool PingSession();

	/* Closes the session. */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Close Session"), Category = "GameJolt|Sessions")
	bool CloseSession();

#pragma endregion

	/* Gets the time of the GameJolt Servers */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fetch Server Time"), Category = "GameJolt|Misc")
	bool FetchServerTime();

	/* Outputs the server time that was fetched with FetchServerTime */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Read Server Time"), Category = "GameJolt|Misc")
	struct FDateTime ReadServerTime();

#pragma region User

	/**
	 * Sends a request to authentificate the user
	 * Call UUEGameJoltAPI::isUserAuthorize / Is User Login to check whether the authorization was successful or not
 	 * @param Name The username - case insensitive
 	 * @param Token The token - case insensitive
 	 */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Login"), Category = "GameJolt|User")
	void Login(FString Name, FString Token);

	/* Is User logged in? */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Is User Login"), Category = "GameJolt|User")
	bool isUserAuthorize();

	/* Logs User off */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Logoff User"), Category = "GameJolt|User")
	void LogOffUser();

	/* Fetch User */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fetch Users"), Category = "GameJolt|User")
	bool FetchUser();

	/* Gets Array of Users */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Array of Users"), Category = "GameJolt|User")
	TArray<FUserInfo> FetchArrayUsers();

#pragma endregion

#pragma region Trophies

	/* Rewards Trophy */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Reward Trophies"), Category = "GameJolt|Trophies")
	bool RewardTrophy(int32 Trophy_ID);

	/* Fetch all trophies */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fetch All Trophies"), Category = "GameJolt")
	void FetchAllTrophies(EGameJoltAchievedTrophies AchievedType);

	/* Fetch Trophies */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fetch Trophies"), Category = "GameJolt|Trophies")
	void FetchTrophies(EGameJoltAchievedTrophies AchievedType, TArray<int32> Trophies_ID);

	/* Get Trophies */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Trophies"), Category = "GameJolt|Trophies")
	TArray<FTrophyInfo> GetTrophies();

#pragma endregion


	/* Score Stuff */

	/* Fetch Scoreboard*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fetch Scoreboard"), Category = "GameJolt|Scoreboard")
	bool FetchScoreboard(int32 ScoreLimit, int32 Table_id);

	/* Gets Scoreboard */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Scoreboard"), Category = "GameJolt|Scoreboard")
	TArray<FScoreInfo> GetScoreboard();

	/* Adds Score to Scoreboard */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Score to Scoreboard"), Category = "GameJolt|Scoreboard")
	bool AddScore(FString UserScore, int32 UserScore_Sort, FString GuestUser, FString extra_data, int32 table_id);

	/**
	 * Returns a list of high score tables for a game.
	 * @return True if it the request succeded and false if it failed
	 **/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fetch Scoreboard Table"), Category = "GameJolt|Scoreboard")
	bool FetchScoreboardTable();

	/** 
	 * Gets a list of high score tables for a game and puts them in an array of FScoreTableInfo structs
	 * @return  A array of FScoreTableInfo structs
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Scoreboard Table"), Category = "GameJolt|Scoreboard")
	TArray<FScoreTableInfo>  GetScoreboardTable();


	/* Utility Functions */

	/* Sends Request */
	UFUNCTION(Blueprintcallable, meta = (Displayname = " Send Request"), Category = "GameJolt|Request|Advanced")
	bool SendRequest(const FString& output, FString url);

	/** Gets nested post data from the object with the specified key
	 * @param key The key of the post data value
	 * @return The value as an UUEGameJoltAPI object reference / pointer
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Data Field", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "GameJolt|Request|Advanced")
	UUEGameJoltAPI* GetObject(const FString& key);

	/** Gets a string from the object with the specified key
	 * @param key The key of the string value
	 * @return The value as a string
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get String Field"), Category = "GameJolt|Request|Advanced")
	FString GetString(const FString& key) const;

	/** Gets a bool from the object with the specified key
	 * @param key The key of the bool value
	 * @return The value as a bool
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Bool Field"), Category = "GameJolt|Request|Advanced")
	bool GetBool(const FString& key) const;

	/** Gets an integer from the object with the specified key
	 * @param key The key of the integer value
	 * @return The value as an integer
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Int Field"), Category = "GameJolt|Request|Advanced")
	int32 GetInt(const FString& key) const;

	/**
	 * Gets a string array of all keys from the post data
	* @return An array with all keys
	*/
	UFUNCTION(Blueprintpure, meta = (Displayname = "Get Object Keys", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "GameJolt|Request|Advanced")
	TArray<FString> GetObjectKeys(UObject* WorldContextObject);

	/* Gets an array with post data with the specified key */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Object Array Field", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "GameJolt|Request|Advanced")
	TArray<UUEGameJoltAPI*> GetObjectArray(UObject* WorldContextObject, const FString& key);

};