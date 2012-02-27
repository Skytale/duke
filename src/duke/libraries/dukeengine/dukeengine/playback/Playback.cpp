#include "Playback.h"

using namespace std;
using namespace boost::chrono;

namespace playback {

// globals
high_resolution_clock s_Clock;
static const duration zero(high_resolution_clock::duration::zero());

/**
 * Returns the time for a frame in ns considering a given framerate
 */
nanoseconds nsPerFrame(unsigned int numerator, unsigned int denominator) {
    assert(denominator!=0);
    if (numerator == 0)
        return zero;
    return nanoseconds(seconds(denominator)) / numerator;
}

nanoseconds nsPerFrame(double frameRate) {
    if (frameRate == 0)
        return zero;
    const int64_t nsPerSecond = nanoseconds(seconds(1)).count();
    const int64_t nsPerFrame = nsPerSecond / frameRate;
    return nanoseconds(nsPerFrame);
}
namespace details {



PlaybackContinuum::PlaybackContinuum(size_t newFrame, high_resolution_clock::duration nsPerFrame, unsigned int minFrame, unsigned int maxFrame) :
    m_NsPerFrame(nsPerFrame), m_EpochFrame(newFrame), m_EpochTime(s_Clock.now()), m_MinFrame(std::min(minFrame, maxFrame)), m_MaxFrame(std::max(minFrame, maxFrame)), m_Frame(
        newFrame) {
}

bool PlaybackContinuum::frameOverrun() const {
//    return false;
    return presentationTimeFor(m_Frame) < s_Clock.now();
}

high_resolution_clock::time_point PlaybackContinuum::presentationTimeFor(unsigned int newFrame) const {
    const int64_t frameDifferences = int64_t(newFrame) - int64_t(m_EpochFrame);
    return m_EpochTime + frameDifferences * m_NsPerFrame;
}

bool PlaybackContinuum::adjustCurrentFrame(bool &frameMissed) {
    frameMissed = false;
    if (m_NsPerFrame == zero)
        return false;

    bool secondStep = false;
    do {
        if (stepFrame())
            return true;
        frameMissed |= secondStep; // if more than one step we missed a frame
        secondStep = true;
    } while (frameOverrun());
    return false;
}

/**
 * return true if a bound is reached
 */
inline bool PlaybackContinuum::stepFrame() {
    assert(m_NsPerFrame!=zero);
    // stop state is not allowed, must be either forward or backward here
    if (m_NsPerFrame > zero) { // forward
        if (m_Frame >= m_MaxFrame)
            return true;
        else
            ++m_Frame;
    } else { // backward
        if (m_Frame <= m_MinFrame)
            return true;
        else
            --m_Frame;
    }
    return false;
}

}  // namespace details

RealtimePlaybackState::RealtimePlaybackState() :
    m_Loop(true), m_MinFrame(0), m_MaxFrame(0), m_NsPerFrame(zero), m_Speed(0), m_State(m_MinFrame, zero, m_MinFrame, m_MaxFrame) {
}

RealtimePlaybackState::RealtimePlaybackState(high_resolution_clock::duration nsPerFrame, unsigned int minFrame, unsigned int maxFrame, bool loop) :
    m_Loop(loop), m_MinFrame(minFrame), m_MaxFrame(maxFrame), m_NsPerFrame(nsPerFrame), m_Speed(0), m_State(m_MinFrame, zero, m_MinFrame, m_MaxFrame) {
}

bool RealtimePlaybackState::adjustCurrentFrame() {
    bool frameMissed = false;
    if (isPlaying() && m_State.adjustCurrentFrame(frameMissed)) { // bound reached
        if (m_Loop) {
            const unsigned int otherBound = frame() == m_MaxFrame ? m_MinFrame : m_MaxFrame;
            m_State = details::PlaybackContinuum(otherBound, nsPerFrame(), m_MinFrame, m_MaxFrame);
        } else {
            stop();
        }
    }
    return frameMissed;
}

void RealtimePlaybackState::play(unsigned int frame, int speed) {
    m_Speed = speed;
    m_State = details::PlaybackContinuum(frame, nsPerFrame(), m_MinFrame, m_MaxFrame);
}

} // namespace playback
