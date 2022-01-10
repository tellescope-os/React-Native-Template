//
//  AudioClientSDK.h
//  AmazonChimeSDKMedia
//
//  Copyright 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import "audio_client_enum.h"

typedef enum
{
    LOGGER_TRACE = 1,
    LOGGER_DEBUG = 2,
    LOGGER_INFO = 3,
    LOGGER_WARNING = 4,
    LOGGER_ERROR = 5,
    LOGGER_FATAL = 6,
    LOGGER_NOTIFY = 7,
} loglevel_t;

typedef NS_ENUM(NSUInteger, AudioClientMetric) {
    clientMicDeviceFramesLostPercent,
    serverPreJbMicPacketsLostPercent,
    serverMicMaxJitterMs,
    serverPostJbMic1sPacketsLostPercent,
    serverPostJbMic5sPacketsLostPercent,
    clientPreJbSpkPacketsLostPercent,
    clientSpkMaxJitterMs,
    clientPostJbSpk1sPacketsLostPercent,
    clientPostJbSpk5sPacketsLostPercent,
};

@interface AppInfo: NSObject
    @property NSString *appName;
    @property NSString *appVersion;
    @property NSString *deviceMake;
    @property NSString *deviceModel;
    @property NSString *platformName;
    @property NSString *platformVersion;
    @property NSString *clientSource;
    @property NSString *chimeSdkVersion;
- (id) initWithAppName:(NSString *)appName
            appVersion:(NSString *)appVersion
            deviceMake:(NSString *)deviceMake
           deviceModel:(NSString *)deviceModel
          platformName:(NSString *)platformName
       platformVersion:(NSString *)platformVersion
          clientSource:(NSString *)clientSource
       chimeSdkVersion:(NSString *)chimeSdkVersion;
@end

// Internal transcript models
typedef NS_ENUM(NSInteger, TranscriptItemTypeInternal) {
    TranscriptItemTypeInternalPronunciation = 1,
    TranscriptItemTypeInternalPunctuation = 2,
};

typedef NS_ENUM(NSInteger, TranscriptionStatusTypeInternal) {
    TranscriptionStatusTypeInternalStarted = 1,
    TranscriptionStatusTypeInternalInterrupted = 2,
    TranscriptionStatusTypeInternalResumed = 3,
    TranscriptionStatusTypeInternalStopped = 4,
    TranscriptionStatusTypeInternalFailed = 5,
};

@interface AttendeeInfoInternal : NSObject
@property (nonatomic, readonly, copy) NSString *attendeeId;
@property (nonatomic, readonly, copy) NSString *externalUserId;
- (instancetype)initWithAttendeeId:(NSString *)attendeeId externalUserId:(NSString *)externalUserId;
@end

@interface TranscriptItemInternal : NSObject
@property (nonatomic, readonly) enum TranscriptItemTypeInternal type;
@property (nonatomic, readonly) int64_t startTimeMs;
@property (nonatomic, readonly) int64_t endTimeMs;
@property (nonatomic, readonly, strong) AttendeeInfoInternal *attendee;
@property (nonatomic, readonly, copy) NSString *content;
@property (nonatomic, readonly) BOOL vocabularyFilterMatch;
- (instancetype)initWithType:(TranscriptItemTypeInternal)type
                 startTimeMs:(int64_t)startTimeMs
                   endTimeMs:(int64_t)endTimeMs
                    attendee:(AttendeeInfoInternal *)attendee
                     content:(NSString *)content
       vocabularyFilterMatch:(BOOL)vocabularyFilterMatch;
@end

@interface TranscriptAlternativeInternal : NSObject
@property (nonatomic, readonly, copy) NSArray<TranscriptItemInternal *> *items;
@property (nonatomic, readonly, copy) NSString *transcript;
- (instancetype)initWithItems:(NSArray<TranscriptItemInternal *> *)items transcript:(NSString *)transcript;
@end

@protocol TranscriptEventInternal
@end

@interface TranscriptResultInternal : NSObject
@property (nonatomic, readonly, copy) NSString *resultId;
@property (nonatomic, readonly, copy) NSString *channelId;
@property (nonatomic, readonly) BOOL isPartial;
@property (nonatomic, readonly) int64_t startTimeMs;
@property (nonatomic, readonly) int64_t endTimeMs;
@property (nonatomic, readonly, copy) NSArray<TranscriptAlternativeInternal *> *alternatives;
- (instancetype)initWithResultId:(NSString *)resultId
                       channelId:(NSString *)channelId
                       isPartial:(BOOL)isPartial
                     startTimeMs:(int64_t)startTimeMs
                       endTimeMs:(int64_t)endTimeMs
                    alternatives:(NSArray<TranscriptAlternativeInternal *> *)alternatives;
@end

@interface TranscriptionStatusInternal : NSObject <TranscriptEventInternal>
@property (nonatomic, readonly) enum TranscriptionStatusTypeInternal type;
@property (nonatomic, readonly) int64_t eventTimeMs;
@property (nonatomic, readonly, copy) NSString *transcriptionRegion;
@property (nonatomic, readonly, copy) NSString *transcriptionConfiguration;
@property (nonatomic, readonly, copy) NSString *message;
- (instancetype)initWithType:(enum TranscriptionStatusTypeInternal)type
                 eventTimeMs:(int64_t)eventTimeMs
         transcriptionRegion:(NSString *)transcriptionRegion
  transcriptionConfiguration:(NSString *)transcriptionConfiguration
                     message:(NSString *)message;
@end

@interface TranscriptInternal : NSObject <TranscriptEventInternal>
@property (nonatomic, readonly, copy) NSArray<TranscriptResultInternal *> *results;
- initWithResults:(NSArray<TranscriptResultInternal *> *)results;
@end

@protocol AudioClientDelegate <NSObject>

@optional

- (void)signalStrengthChanged:(NSArray*)signalStrengths;

- (void)audioClientStateChanged:(audio_client_state_t)audio_client_state
                         status:(audio_client_status_t)status;

- (void)volumeStateChanged:(NSArray*)volumes;

- (void)audioMetricsChanged:(NSDictionary*)metrics;

- (void)attendeesPresenceChanged:(NSArray*)attendees;

- (void)transcriptEventsReceived:(NSArray*)events;

@end

@interface AudioClient : NSObject
{
@private audio_client_t *_client;
}

@property (nonatomic, retain) NSObject <AudioClientDelegate> *delegate;

// startSession method that passes AppInfo containing iOS SDK metadata to Audioclient.
- (audio_client_status_t)startSession:(NSString *)host
                             basePort:(NSInteger)port
                               callId:(NSString*)callId
                            profileId:(NSString*)profileId
                       microphoneMute:(BOOL)mic_mute
                          speakerMute:(BOOL)spk_mute
                          isPresenter:(BOOL)presenter
                         sessionToken:(NSString *)tokenString
                           audioWsUrl:(NSString *)audioWsUrl
                       callKitEnabled:(BOOL)callKitEnabled
                              appInfo:(AppInfo *)appInfo
;

// Legacy startSession method without passing AppInfo for backward compatibility.
- (audio_client_status_t)startSession:(NSString *)host
                             basePort:(NSInteger)port
                               callId:(NSString*)callId
                            profileId:(NSString*)profileId
                       microphoneMute:(BOOL)mic_mute
                          speakerMute:(BOOL)spk_mute
                          isPresenter:(BOOL)presenter
                         sessionToken:(NSString *)tokenString
                           audioWsUrl:(NSString *)audioWsUrl
                       callKitEnabled:(BOOL)callKitEnabled
;

- (NSInteger)stopSession;

- (BOOL)isSpeakerOn;

- (BOOL)setSpeakerOn:(BOOL)value;

- (NSInteger) stopAudioRecord;

- (BOOL)isMicrophoneMuted;

- (NSInteger)setMicrophoneMuted:(BOOL)mute;

- (BOOL)isBliteNSSelected;

- (NSInteger)setBliteNSSelected:(BOOL)bliteSelected;

- (void)setPresenter:(BOOL)presenter;

- (void)remoteMute;

- (void) audioLogCallBack:(loglevel_t)logLevel
                      msg:(NSString*)msg;
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
- (void)beginOnHold;
- (void)endOnHold;
#endif
+ (AudioClient *)sharedInstance;

@end

@interface AttendeeUpdate: NSObject

@property NSString *profileId;
@property NSString *externalUserId;
@property NSNumber *data;

- (id) initWithProfileId:(NSString *)profile_id
          externalUserId:(NSString *)external_user_id
                    data:(NSNumber *)data;

@end

