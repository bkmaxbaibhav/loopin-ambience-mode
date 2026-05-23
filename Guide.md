================================================================================
        LOOPIN AMBIENCE MODE — COMPLETE DESIGN & EXPERIENCE GUIDE
        For Developers Who Feel Music Visually
        Written as a Human Eye Health Guide + UX Expert Perspective
================================================================================


--------------------------------------------------------------------------------
SECTION 1 — THE PHILOSOPHY BEHIND THE APP
--------------------------------------------------------------------------------

Before anything else, understand what your app is competing with.
It is not competing with Winamp visualizers or screensavers.
It is competing with the feeling of sitting in a room where the lights
match the mood of your music perfectly without you having to think about it.

That is the emotional target. Effortless immersion.

The user should never feel like they are "watching" the effect.
They should feel like the effect is just... there. Breathing with the music.
When that happens, they stop noticing the borders and start feeling the room.

This is the campfire principle:
A campfire flickers constantly yet people stare at it for hours without strain.
It is never brighter than its surroundings.
Its changes are organic, not mechanical.
There is a rhythm underneath the randomness.
It rewards passive watching.

Every single design decision in this guide comes back to this principle.


--------------------------------------------------------------------------------
SECTION 2 — HUMAN EYE HEALTH FUNDAMENTALS
--------------------------------------------------------------------------------

You are placing light in the peripheral vision zone.
The edges of a screen sit exactly where peripheral vision is most active.
Peripheral vision is MORE sensitive to flicker and motion than central vision.
This makes your app uniquely capable of both delight and damage.

Here are the biological facts you must design around:


2.1 THE FLICKER THRESHOLD

  Below 3 Hz  — perceived as slow pulsing, calming, safe
  3 Hz to 10 Hz — noticeable flicker, tolerable in short sessions
  10 Hz to 50 Hz — uncomfortable flicker zone, headache trigger
  Above 50 Hz — perceived as smooth glow by most people, safe
  
  The dangerous zone for your app is 10 to 50 Hz flicker in brightness.
  Color changes at that speed are less dangerous than brightness changes.
  NEVER strobe pure white or near-white at 10 to 50 Hz.
  Safe flicker = slow organic pulses OR very fast smooth transitions above 50 Hz.


2.2 THE BRIGHTNESS RULE

  The ambient border must never exceed the average luminance of the room.
  A glowing border in a dark room forces the pupil to constantly fight contrast.
  Over 30 minutes this causes pupil fatigue, a dull ache behind the eyes.
  
  Design rule: Default brightness should be 40 to 60 percent of maximum.
  Let the user push it higher, but warn them at 80 percent and above.
  Better approach: offer an AUTO mode that reads system brightness
  and scales the border brightness to match.


2.3 COLOR SATURATION FATIGUE

  Pure saturated colors at 100 percent saturation are tiring after 20 minutes.
  The eye's cone cells handling red, green, and blue get individually fatigued.
  This is why staring at a red wall makes you see cyan ghosts.
  
  Design rule: Keep saturation between 55 and 80 percent for long sessions.
  Reserve 90 to 100 percent saturation only for brief peak moments like
  a heavy bass hit or a dramatic music drop.
  Return to lower saturation within 500 milliseconds.


2.4 THE PERIPHERAL MOTION LIMIT

  Fast lateral movement along the border edges triggers involuntary tracking.
  The eye tries to follow moving things it sees in peripheral vision.
  This is a survival reflex. You cannot turn it off.
  
  Design rule: Motion that travels along the border should move at a pace
  that the eye can choose to ignore. Slow enough to feel smooth.
  If the movement is too fast, the eye tries to catch it and fails repeatedly.
  That repeated failed tracking is what causes the tired heavy eyelid feeling.
  
  Safe lateral motion speed: travel the full screen border in 2 to 8 seconds.
  Anything faster should only happen on beat hits and only for 200 milliseconds.


2.5 THE 90 SECOND HABITUATION WINDOW

  The human eye adapts to any repeating pattern in about 90 seconds.
  After 90 seconds of the same effect, the brain classifies it as background.
  This is good for eye health. It means the eye stops working to process it.
  But it is bad for engagement. The user stops feeling it.
  
  Design rule: Every mode must have a subtle variation cycle of 60 to 120 seconds.
  Not a dramatic change. Just enough variation that the brain cannot fully
  classify the pattern as static background.
  A slight tempo drift. A gentle hue rotation. A breathing intensity shift.
  The user will not see the change consciously but will feel that the app is alive.


2.6 THE SESSION COMFORT FADE

  After 45 minutes of runtime, invisibly apply these changes:
  - Reduce overall brightness by 12 percent
  - Shift color temperature 200 Kelvin warmer (toward amber)
  - Slow all transition speeds by 15 percent
  - Reduce maximum saturation by 10 percent
  
  After 90 minutes apply another layer of the same adjustments.
  
  Users will not notice. But they will feel that the app is gentle on them.
  That subconscious feeling of being taken care of builds loyalty.
  This is exactly what f.lux does for screen brightness and why people love it.


--------------------------------------------------------------------------------
SECTION 3 — COLOR CHOICES AND PALETTES
--------------------------------------------------------------------------------

Color is the most powerful tool you have. Choose with intention.


3.1 THE CORE PRINCIPLE OF AMBIENT COLOR

  Ambient light colors are not the same as UI colors.
  UI colors need to be readable and precise.
  Ambient light colors need to feel, not communicate.
  
  The best ambient colors sit at medium brightness and medium saturation.
  They are colors that, if you walked into a room bathed in that color,
  you would feel something without knowing exactly what the source was.


3.2 THE BASE PALETTE — SAFE COLORS FOR LONG SESSIONS

  These colors are low fatigue and pleasant for extended watching.
  All values are in HSL format: Hue, Saturation, Lightness.
  
  Deep Violet       — HSL(270, 60%, 45%)   Calm, focused, late night coding
  Ocean Blue        — HSL(210, 65%, 40%)   Clear, fresh, morning sessions
  Forest Teal       — HSL(175, 55%, 38%)   Grounded, natural, long sessions
  Warm Amber        — HSL(38, 65%, 45%)    Cozy, safe, eyes love warm light
  Soft Rose         — HSL(340, 55%, 50%)   Gentle, smooth, lo-fi moods
  Muted Indigo      — HSL(245, 50%, 42%)   Deep concentration, focus states
  
  These are your default palette. Use them as the resting state.
  Music pushes them brighter, more saturated, more dynamic.
  Then they breathe back to these values.


3.3 THE ACCENT PALETTE — FOR PEAK MOMENTS ONLY

  These are used for beat hits, drops, high energy peaks.
  Never hold these colors for more than 400 milliseconds.
  
  Electric Cyan     — HSL(185, 90%, 55%)   High frequency peaks, treble hits
  Neon Magenta      — HSL(310, 85%, 55%)   Mid-range punch, snare hits
  Pure Amber        — HSL(40, 90%, 55%)    Bass warmth, kick drum
  Ice White         — HSL(210, 30%, 85%)   Climax moment, music drop
  Deep Crimson      — HSL(350, 75%, 45%)   Heavy bass, sub frequencies
  
  The rule is: arrive fast, leave slowly. 
  Hit the accent color in 30 to 80 milliseconds.
  Decay back to base color in 300 to 600 milliseconds.
  This attack and release pattern is what makes it feel musical.


3.4 GENRE-BASED COLOR TEMPERATURE MAPPING

  Different music genres carry different emotional temperatures.
  Your reactive mode should ideally understand frequency signatures
  and map them to appropriate color temperatures.
  
  Electronic / EDM     — Cool blues, cyans, electric purples
                         High saturation, fast transitions okay
  
  Lo-fi / Chill        — Warm ambers, muted roses, soft indigo
                         Low saturation, very slow gentle transitions
  
  Rock / Metal         — Deep crimsons, electric blue-whites, dark oranges
                         High contrast, sharp attack, medium decay
  
  Jazz / Blues         — Warm golds, deep teals, muted purples
                         Flowing, no hard hits, smooth rolls
  
  Classical            — Soft whites, pale golds, gentle lavenders
                         Extremely slow transitions, breathing only
  
  Hip Hop / Trap       — Deep purples, bass-driven amber, cool mids
                         Strong bass response, controlled high end


3.5 THE COLOR HARMONY RULES

  When reacting to stereo channels separately (left and right borders),
  the two colors must be harmonically related or they will clash.
  
  Good pairings for left and right channels:
  - Complementary pair: Violet left, Amber right
  - Analogous pair: Blue left, Teal right
  - Split complementary: Indigo left, Gold-Orange right
  
  Never use:
  - Pure red next to pure green (this is a known eye strain combination)
  - High saturation colors on opposite sides simultaneously at full brightness
  - Two colors that create a strong afterimage effect when viewed together


3.6 THE DOLBY VISION COLOR APPROACH

  Dolby Vision achieves its signature look through three things:
  
  First — Peak brightness that is extremely localised.
  The brightest point is tiny and surrounded by much darker areas.
  In your app: let the brightest glow be concentrated at corners or
  a single edge point, not the entire border at once.
  
  Second — HDR-style color volume.
  Colors appear at full saturation even at very high or very low brightness.
  Standard displays wash out saturation in very bright or very dark colors.
  Dolby Vision does not. For your app: when a bass hit drives brightness up,
  maintain or even increase saturation instead of washing out to white.
  
  Third — The specular highlight feel.
  Dolby Vision content has small regions of extreme brightness
  that feel like actual light sources rather than illuminated surfaces.
  For your app: create a small traveling point of high intensity
  within the border glow, like a hot spot or lens flare moving with the music.
  The border glows softly but inside it there is a brighter concentrated core.
  
  This three-part combination gives that premium cinematic feel
  that makes users feel they have something special running on their screen.


--------------------------------------------------------------------------------
SECTION 4 — ANIMATION DESIGN PRINCIPLES
--------------------------------------------------------------------------------

4.1 THE ATTACK AND RELEASE CONCEPT

  Every animation in your app should be thought of as a sound envelope.
  ADSR: Attack, Decay, Sustain, Release.
  
  Attack  — How fast the animation reaches peak intensity
  Decay   — How fast it settles from peak to sustain level
  Sustain — The level it holds during a note or beat
  Release — How slowly it fades after the trigger ends
  
  This is not a metaphor. It is a direct mapping from audio to visual.
  
  For a kick drum hit:
  Attack = 20 to 40 milliseconds (very fast, almost instant)
  Decay = 80 to 120 milliseconds (quick settle)
  Sustain = low, kick is a short sound
  Release = 200 to 400 milliseconds (gentle fade)
  
  For a sustained synthesizer pad:
  Attack = 300 to 600 milliseconds (slow build)
  Decay = minimal
  Sustain = high, the pad holds
  Release = 800 to 1500 milliseconds (long gentle fade)
  
  When your animation envelopes match the audio envelopes,
  users feel the sync without being able to explain why it feels so good.


4.2 THE THREE LAYERS OF ANIMATION

  Every visual mode should have three simultaneous layers.
  These layers run at different timescales and serve different purposes.
  
  Layer 1 — The Breath Layer (0.3 to 2 Hz)
  This is the slowest layer. It gives the effect a living, breathing quality.
  Even when no music is loud, this layer gently pulses the overall brightness
  by 10 to 20 percent up and down. It never fully stops.
  This is what prevents the effect from ever looking "frozen" or dead.
  
  Layer 2 — The Rhythm Layer (tempo-synced)
  This tracks the beat of the music. It follows the kick, the snare, the pulse.
  This is the primary reactive layer that users consciously see and feel.
  It operates at music tempo, typically 60 to 180 BPM.
  
  Layer 3 — The Texture Layer (fast, subtle)
  This is a very subtle, fast-moving variation that adds grain or shimmer.
  It runs at 8 to 30 Hz but at very low intensity, 5 to 15 percent variation.
  It is what separates a flat glowing bar from a rich glowing bar.
  Think of it as the difference between a LED strip and a neon tube.
  Neon tubes have organic internal variation. They feel alive.


4.3 EASING FUNCTIONS — THE DIFFERENCE BETWEEN CHEAP AND PREMIUM

  Linear animation = cheap, robotic, unpleasant for long viewing
  Eased animation = natural, organic, pleasant
  
  For brightness rising (attack phase):
  Use ease-out cubic. Fast start, gentle arrival at peak.
  This feels like a light switching on with intention.
  
  For brightness falling (release phase):
  Use ease-in-out sine. Smooth departure, smooth landing.
  This feels like light naturally fading, like a sun setting.
  
  For color transitions:
  Never transition through grey or desaturated midpoints.
  Always transition through a related hue to maintain richness.
  Going from blue to orange should pass through teal to amber,
  not through grey-white.
  
  For position-based effects (traveling light):
  Use ease-in-out for the travel. Slow start, fast middle, slow arrival.
  This matches how the eye tracks moving objects comfortably.


4.4 FRAME TIMING RECOMMENDATIONS

  15 FPS — Minimum for basic reactive glow. Visibly choppy. Not recommended.
  30 FPS — Acceptable for slow ambient modes. Soft_aura works here.
  60 FPS — The standard. All modes should look good at 60 FPS.
  120 FPS — Where fast beat_bloom and corner_hits become buttery smooth.
  240 FPS — Diminishing returns for glow effects. Reserve for special modes.
  
  Important: Match your animation update rate to the audio analysis rate.
  If you analyze audio every 16 milliseconds (60 Hz analysis),
  running your render at 240 FPS gives you 4 identical frames per analysis.
  That wastes GPU cycles. Analysis rate and render rate should be paired.
  
  Recommended pairing: 60 Hz audio analysis with 60 FPS render.
  For premium feel: 120 Hz audio analysis with 120 FPS render.


--------------------------------------------------------------------------------
SECTION 5 — ANIMATION MODE DESIGN IN DETAIL
--------------------------------------------------------------------------------

5.1 SOFT_AURA — THE DEFAULT MODE

  This is your safest, most comfortable mode.
  It should be the mode users discover first and return to most often.
  
  Character: The border glows like a colored fog or mist.
  No hard edges to the glow. Everything bleeds softly.
  The entire border breathes together as one unified field.
  
  Behavior:
  - Base pulse at 0.5 to 1 Hz, ±15 percent brightness variation
  - Reacts to overall audio energy (RMS volume level)
  - Color shifts slowly with the dominant frequency in the music
  - Bass frequencies = warmer tones, pull toward amber
  - Treble frequencies = cooler tones, pull toward cyan or blue
  - Transition speed between colors: 800 milliseconds minimum
  
  Eye health rating: Excellent. Suitable for unlimited session length.
  
  The interesting secret of soft_aura:
  Make the four corners slightly brighter than the edges between them.
  This creates a subtle vignette-like framing that draws the user's
  attention inward toward the screen content, not outward to the border.
  The border enhances focus rather than competing for attention.


5.2 SPECTRUM_FLOW — THE SHOW-OFF MODE

  This is your visually richest mode. The one users demo to friends.
  Each section of the border represents a different frequency band.
  
  Character: A flowing river of color where different hues correspond
  to different frequencies, traveling around the screen.
  
  Frequency to position mapping approach one — static zones:
  Left border = bass frequencies (sub 200 Hz)
  Bottom border = low-mid frequencies (200 to 800 Hz)
  Right border = mid-high frequencies (800 Hz to 5 kHz)
  Top border = high frequencies (5 kHz to 20 kHz)
  
  Frequency to position mapping approach two — flowing:
  Low frequencies create color events at the bottom corners
  that travel upward along both sides simultaneously.
  High frequencies create color events at the center of the top border
  that travel outward and downward.
  The two flows meet, mix at corners, and create emergent patterns.
  
  Approach two is more visually interesting but harder to implement.
  
  Travel speed: 2 to 5 seconds for a full border loop at normal tempo.
  Speed up to 0.8 to 1.5 seconds during high energy sections.
  
  Eye health consideration: The flowing motion should always travel
  in one consistent direction. Do not reverse direction rapidly.
  Directional reversals trigger the tracking reflex and cause fatigue.
  If you want an energy burst effect, pause the flow briefly then resume.


5.3 BEAT_BLOOM — THE FEEL-THE-MUSIC MODE

  This mode makes the border feel like a speaker physically vibrating.
  
  Character: On each beat hit, the entire border or specific sections
  bloom outward in brightness, then fade back like a ripple in water.
  
  The bloom shape matters:
  A flat top bloom (hit max brightness and hold briefly before falling)
  feels mechanical and aggressive. Good for metal or heavy bass.
  
  A peaked bloom (rise to maximum and immediately begin falling)
  feels more organic and musical. Good for most genres.
  
  A double-peak bloom (rise, slight dip, second smaller peak, then fall)
  feels like a speaker physically moving forward and backward.
  This is the most tactile feeling bloom. Extremely satisfying on kick drums.
  
  Frequency zones for bloom triggering:
  Sub-bass hits (below 80 Hz) = whole border bloom, warm amber/red
  Kick drum (80 to 200 Hz) = bottom and side bloom, amber push
  Snare (200 to 500 Hz) = left and right bloom, cool flash
  Hi-hat / Cymbal (above 8 kHz) = top border shimmer, cool white touch
  
  Anti-fatigue rule:
  After three consecutive fast blooms, force a minimum 150 millisecond
  cooldown where blooms are damped to 60 percent maximum intensity.
  This prevents the repeated flash fatigue that causes headaches.
  The user will experience it as the music "breathing" which feels good.


5.4 CORNER_HITS — THE DRAMATIC MODE

  This mode treats the four corners as focal points of energy.
  
  Character: Energy arrives at corners and radiates outward along edges.
  Corners are where the eye naturally rests when looking at a screen frame.
  Lighting them creates a sense of contained energy, like electricity at nodes.
  
  Corner behavior:
  When a strong transient hits (kick, snare, drum hit):
  The nearest corner to the frequency range origin flashes bright,
  then sends a wave of light along the edges in both directions.
  The two waves from a corner travel along their respective edges
  and fade before reaching the next corners.
  
  If the transient is strong enough, waves from two corners
  meet at the center of an edge, creating a point of maximum brightness.
  Then the glow pulses once at that meeting point and fades.
  
  This creates a visual grammar: corners are origins, edges are channels,
  meetings are climaxes. Users learn this grammar intuitively.
  After a few minutes they start anticipating the meeting points.
  That anticipation is a powerful engagement hook.
  
  Eye health: Corners are in the far peripheral zone.
  Keep corner maximum brightness at 70 to 75 percent.
  The flash there is very close to where the eye is most sensitive.


5.5 THE DOLBY VISION PREMIUM MODE — DESIGN CONCEPT

  This is a mode that combines everything into a cinematic experience.
  
  Core visual idea:
  The border has three simultaneous phenomena happening:
  
  Phenomenon one — The Ambient Field:
  A soft, low-brightness color fills the entire border.
  This is the base mood color, reacting slowly to overall audio energy.
  Think of it as the room light color.
  
  Phenomenon two — The Dynamic Wave:
  Brighter, more saturated waves travel through the ambient field.
  These are triggered by mid and high frequency content.
  They have a defined shape: brighter leading edge, trailing fade.
  Like a wave of light moving through fog.
  
  Phenomenon three — The Specular Point:
  A small region of extreme brightness, much brighter than everything else.
  This travels along the border following the highest energy transient.
  It leaves a very brief trail (100 to 150 ms) as it moves.
  The specular point is the equivalent of the Dolby Vision highlight.
  It exists to prove to the eye that the system can do more.
  Even when it is not visible, the eye expects it might appear again.
  That expectation creates engagement.
  
  Color rules for Dolby Vision mode:
  Ambient field: 30 to 45 percent lightness, 50 to 60 percent saturation
  Dynamic wave: 50 to 65 percent lightness, 70 to 80 percent saturation
  Specular point: 80 to 92 percent lightness, 75 to 85 percent saturation
  
  The specular point maintains high saturation even at high brightness.
  This is the critical Dolby Vision trick. It never washes out to white.
  It gets brighter while staying colored. Like a colored star, not a floodlight.


--------------------------------------------------------------------------------
SECTION 6 — STEREO AND AUDIO CHANNEL SYNCHRONIZATION
--------------------------------------------------------------------------------

6.1 WHY STEREO MATTERS FOR AMBIENCE

  Most ambient lighting apps treat audio as mono.
  They react to overall loudness. This is the minimum viable approach.
  
  True stereo awareness transforms the app from a reactive light show
  into a spatial audio visualizer. The room literally sounds different
  on left versus right and your borders reflect that.
  This is the feature that makes audiophiles love your app.


6.2 LEFT CHANNEL — RIGHT CHANNEL — MAPPING PHILOSOPHY

  Physical mapping (most intuitive):
  Left audio channel = Left border
  Right audio channel = Right border
  Combined (mid) channel = Top and bottom borders
  
  This means when a guitar panned left plays a riff,
  only the left border dances. The right border rests.
  The listener feels the stereo image physically around their screen.
  
  Emotional mapping (more cinematic):
  Left channel = Bottom-left corner, left border, bleeds into bottom border
  Right channel = Bottom-right corner, right border, bleeds into bottom border
  Mid channel = Top border (the shared, "center stage" space)
  
  This creates a stage metaphor. Instruments spread across the bottom
  and the shared melody lives at the top.


6.3 STEREO WIDTH VISUALIZATION

  Calculate stereo width as the difference between left and right channels.
  When left and right are identical (mono content), width = 0.
  When left and right are opposite (maximum stereo), width = 1.
  
  Use stereo width to control a visual spread parameter:
  Width near 0 = colors on left and right are very similar, nearly identical
  Width near 1 = colors on left and right diverge significantly
  
  This makes stereo-mixed music feel spacious on screen.
  A wide synthesizer pad fills the screen with two different colors.
  A mono vocal locks both sides to the same color.
  Users will feel this even if they cannot describe it.


6.4 THE MID CHANNEL (MONO CONTENT)

  The mid channel is Left plus Right divided by two.
  It captures everything that is centered in the stereo mix:
  vocals, kick drum, bass guitar, main melody.
  
  The mid channel should drive the top and bottom borders.
  These are the "center stage" positions.
  
  When the vocalist sings, the top border responds.
  When the kick drum hits, the bottom border responds.
  Meanwhile the left and right borders handle the stereo field instruments.
  
  The user gets a complete spatial representation:
  Top = melody and center content
  Bottom = rhythm and bass content
  Left and Right = the stereo field, panning, ambience


6.5 THE SIDE CHANNEL (STEREO DIFFERENCE)

  The side channel is Left minus Right divided by two.
  It captures everything that is exclusive to one side:
  panning effects, room reverb tails, stereo wideners, chorus effects.
  
  The side channel is excellent for driving subtle shimmer or texture
  on the left and right borders independently.
  When a reverb tail decays and drifts from center to the sides,
  the borders will catch it and glow faintly with that spatial information.
  
  This makes the app respond to mixing decisions in the music.
  A well-mixed track with spatial depth will look more interesting
  than a flat over-compressed track. Your app becomes a mixing quality indicator.


6.6 FREQUENCY BAND ALLOCATION FOR BORDERS

  Here is a comprehensive frequency mapping for all four borders:
  
  TOP BORDER
  Primary driver: Mid channel, high frequencies (3 kHz to 20 kHz)
  Character: Treble shimmer, vocal presence, cymbal air
  Color tendency: Cooler tones, aqua, pale blue, icy white touches
  Motion: Subtle shimmer, no heavy movement
  
  BOTTOM BORDER
  Primary driver: Mid channel, low frequencies (20 Hz to 300 Hz)
  Character: Bass weight, kick drum, sub energy
  Color tendency: Warmer tones, amber, deep orange, dark red for sub
  Motion: Heavy pulse on beat, slow movement otherwise
  
  LEFT BORDER
  Primary driver: Left audio channel, full frequency range
  Secondary: Side channel energy
  Character: Whatever is panned left in the music
  Color tendency: Follows the left channel mood
  Motion: Mirrors audio panning dynamics
  
  RIGHT BORDER
  Primary driver: Right audio channel, full frequency range
  Secondary: Side channel energy
  Character: Whatever is panned right in the music
  Color tendency: Follows the right channel mood, harmonically paired with left
  Motion: Mirrors audio panning dynamics


6.7 BALANCE EVENTS — SPECIAL TRIGGERS

  A balance event is when energy dramatically shifts from center to one side
  or from one side to the other. Panning sweeps in music cause this.
  
  When a balance event is detected (energy crossing from left to right
  or right to left over 200 to 800 milliseconds):
  Create a traveling light that moves from one border to the other
  across the top or bottom border.
  
  This visually traces the panning sweep of the instrument.
  If a synthesizer sweeps from left to right across the stereo field,
  a trail of light moves from the left border, across the top, to the right border.
  
  This is an extremely satisfying moment that makes users say "whoa."
  It reveals the spatial geometry of the music in a way that feels magical.


6.8 MONO COMPATIBILITY CHECK

  Some systems play audio in mono (single speaker, bluetooth small speakers).
  When mono is detected (left and right channels are identical):
  
  Disable independent left and right border behavior.
  Treat all four borders as one unified surface.
  Use the full frequency range to drive a unified breathing glow.
  Add the stereo width visualization in reverse: borders feel "closer together."
  
  The user should not experience a broken mode. Mono should feel intentional.


--------------------------------------------------------------------------------
SECTION 7 — ANIMATION SWITCHING — TRIGGERS AND TRANSITION POINTS
--------------------------------------------------------------------------------

7.1 THE WRONG WAY TO SWITCH ANIMATIONS

  Hard cuts between visual modes are jarring and feel cheap.
  They break immersion. The user is suddenly aware they are watching software.
  
  Never switch visual modes with an instant transition.
  Never switch modes on a user keypress without a visual transition.
  Never switch modes randomly without musical justification.


7.2 THE RIGHT TRIGGER POINTS FOR MODE SWITCHING

  Mode switches should feel like they were caused by the music, not the user.
  
  Energy Threshold Triggers:
  When overall audio energy rises above a sustained threshold for 3 seconds,
  begin transitioning toward a more energetic mode.
  When energy drops below a low threshold for 5 seconds,
  begin transitioning toward a calmer mode.
  
  Silence Break Triggers:
  When audio resumes after a silence of more than 2 seconds,
  this is a natural restart point. An ideal moment to switch modes.
  Music producers often use silence as a structural tool.
  Your mode changes can align with the producer's intent.
  
  Transient Density Triggers:
  Count fast transients (beat hits) per second.
  Below 1 per second = calm mode appropriate (soft_aura)
  1 to 3 per second = rhythmic mode appropriate (beat_bloom)
  Above 3 per second = energetic mode appropriate (spectrum_flow)
  Sudden very dense transients = corner_hits mode, then return
  
  Harmonic Change Triggers:
  When the dominant frequency shifts dramatically (key change, genre break),
  this is a natural transition moment.
  The visual mode can shift with the musical mood.


7.3 HOW TO EXECUTE A MODE TRANSITION

  Phase 1 — Preparation (500 milliseconds before switch):
  Begin very slowly increasing the brightness of the target mode's
  visual signature at 10 to 20 percent opacity. Ghost it in.
  
  Phase 2 — The Cross Dissolve (800 milliseconds to 1500 milliseconds):
  Fade the current mode out while fading the new mode in.
  The dissolve should follow the audio envelope.
  If there is a strong beat during the dissolve, let the new mode's
  response to that beat accelerate the transition completion.
  
  Phase 3 — Arrival Bloom (200 milliseconds after switch):
  Once the new mode is primary, let it respond to the current audio
  with a slightly enhanced intensity for the first 2 to 3 seconds.
  This is a brief honeymoon period for the new mode.
  It says: here I am, look what I can do.
  Then it settles to normal intensity.
  
  Total transition time: 1.5 to 2.5 seconds.
  This feels like a natural mood shift, not a software mode change.


7.4 THE WAITING HOOK — KEEPING USERS THROUGH QUIET SECTIONS

  The biggest risk to session length is a quiet section in the music.
  When the music drops in energy, the borders dim.
  A boring border makes the user think about closing the app.
  
  Solution: The Anticipation State.
  
  When energy drops low, do not simply dim and wait.
  Instead, introduce a slow, mysterious idle animation.
  The border should do a very slow breathing pulse,
  with an occasional dim traveling shimmer as if something is building.
  
  The shimmer should have a sense of direction and intention.
  It should look like energy is gathering, not dissipating.
  
  When the music returns, the gathered energy blooms outward.
  The contrast between the quiet anticipation and the energetic release
  creates one of the most satisfying moments in the entire experience.
  
  Users will sit through 30 seconds of quiet music
  just to see what the bloom looks like when it finally releases.
  That is the "waiting for the next effect" feeling you asked for.


7.5 THE SURPRISE MOMENT ARCHITECTURE

  To prevent habituation and keep users engaged over long sessions,
  design a rare event system.
  
  Common events: Every few seconds. The normal reactive behavior.
  Uncommon events: Every 30 to 90 seconds. A slightly enhanced response.
  Rare events: Every 3 to 8 minutes. Something genuinely beautiful and unexpected.
  
  Rare event examples:
  - The full border slowly cycles through a complete rainbow over 8 seconds
  - All four corners simultaneously bloom and meet at edge centers
  - The border briefly shifts to a completely different color palette
    then fades back as if the room changed color for a moment
  - A single traveling specular point does a full loop around the entire border
  - A brief high-intensity bloom covers the entire screen in soft color
    for 1 second then pulls back, like a camera flash in reverse
  
  These rare events should feel like rewards.
  Users who run the app for long sessions get to see things
  that short-session users never see.
  This is a powerful retention mechanism.
  The app has secrets. Keep using it to discover them.


--------------------------------------------------------------------------------
SECTION 8 — INTERESTING SHADERS AND VISUAL TEXTURES
--------------------------------------------------------------------------------

8.1 WHAT MAKES A SHADER INTERESTING FOR AMBIENT LIGHT

  A flat glowing rectangle is boring. It communicates one thing: brightness.
  An interesting shader communicates richness, depth, and life.
  
  The key is controlled imperfection.
  Perfect, mathematically uniform glow looks like a debug rectangle.
  Slightly irregular, organically varying glow looks like actual light.


8.2 THE ORGANIC NOISE LAYER

  Underneath every visual mode, run a very subtle noise field.
  This is a slowly evolving pattern of very minor brightness variations.
  Imagine the texture of light projected through moving water.
  
  The noise should:
  - Move slowly (0.1 to 0.3 Hz evolution speed)
  - Have very low amplitude (3 to 8 percent brightness variation)
  - Be smoothly interpolated (no sharp edges in the noise)
  - Be synchronized with the audio: louder music = slightly more active noise
  
  Users will never consciously see this noise layer.
  But when you remove it, the glow will suddenly look flat and fake.
  It is the difference between a neon sign and an LED tape.
  One has organic internal variation. One does not.


8.3 THE EDGE FALLOFF SHADER

  The glow should be brightest at the very edge and fade inward.
  But the falloff curve determines the character of the light.
  
  Linear falloff: Harsh, industrial, unpleasant for long viewing
  Quadratic falloff: Better, but still a bit mechanical
  Exponential falloff: Most natural, mimics how real light disperses
  Custom curve: Bright for first 20 percent then exponential decay for the rest
  
  The last option creates a distinct bright "core" at the edge
  with a long soft "halo" extending inward.
  This is how neon lights and LED strips actually look in photographs.
  It is immediately recognizable as real light behavior.


8.4 THE CHROMATIC ABERRATION EFFECT

  Real light sources create chromatic aberration at their edges.
  This is the slight separation of red, green, and blue components.
  
  For a purple border glow, the chromatic aberration would show
  a very faint blue halo slightly beyond the main glow
  and a very faint red halo slightly inside the main glow.
  
  Apply this at very low intensity (2 to 5 pixel offset, 20 to 30 percent opacity).
  It makes the glow look like a real light source photographed with a real lens.
  The brain immediately interprets this as "actual light" rather than "software effect."
  This is a single visual trick that dramatically elevates perceived quality.


8.5 THE BLOOM GRADIENT SHADER

  On strong beat hits, the bloom should not be a uniform brightness increase.
  It should have a gradient character.
  
  For a bottom border bass bloom:
  The center of the bottom border blooms brightest.
  The bloom diminishes as it approaches the corners.
  The corners receive only about 40 percent of the center brightness.
  
  This creates a "speaker cone" visual metaphor.
  The energy radiates from the center like a speaker pushing air outward.
  It is a deeply satisfying visual that matches the audio physics intuitively.


8.6 THE HEAT DISTORTION SHIMMER

  For high-energy moments, add a very subtle shimmer to the bright edge.
  This is like heat distortion above a hot surface.
  
  Extremely small, fast, irregular brightness variations at the very edge.
  Only visible at peak brightness moments.
  Lasts only 100 to 200 milliseconds after a strong hit.
  
  It gives the impression that the border light is so intense
  it is actually affecting the space around it.
  A quality signal: your light is powerful enough to distort reality.


--------------------------------------------------------------------------------
SECTION 9 — THE EXPERIENCE ARC — SESSION DESIGN
--------------------------------------------------------------------------------

9.1 THE FIRST 30 SECONDS

  A user who opens your app for the first time must feel something
  within the first 30 seconds or they will close it.
  
  Default to soft_aura mode immediately.
  If no audio is detected, run a very slow, beautiful idle animation.
  The idle should be so calming that users want to play music
  just to see how it responds.
  
  When audio first starts, let the first response be slightly enhanced
  compared to normal steady-state behavior.
  The greeting bloom. One moment of slightly more drama than usual.
  Then settle to normal levels.


9.2 THE FIRST 5 MINUTES

  This is the discovery phase. The user is learning what the app does.
  Do not overwhelm them with mode changes.
  Let one mode run long enough that they understand its language.
  Then, at a natural musical transition point, introduce a different mode.
  
  The user should think: oh interesting, it changed.
  Not: why did it randomly do that?


9.3 THE 20 MINUTE MARK

  By 20 minutes, the app should have revealed at least two visual modes.
  The user is now "in a session." They have made a commitment.
  
  This is the time to introduce a rare event for the first time.
  Something small but beautiful. A gentle surprise.
  A full border color sweep. A synchronized corner simultaneous bloom.
  Something that makes them look up from their work and think: oh, nice.
  
  Then it is gone. Back to normal. They are not sure exactly what they saw.
  They will keep it running to see if it happens again.


9.4 THE LONG SESSION EXPERIENCE

  After 45 minutes, the comfort fade engages (described in Section 2.6).
  The experience becomes gentler, warmer, more sustainable.
  
  After 90 minutes, the rare event frequency should slightly decrease.
  The app becomes even more ambient, even more background.
  It is now furniture. It is part of the room.
  
  This is the ultimate goal. When your app becomes furniture,
  it is impossible to use the computer without it.
  That is how you build a loyal user base.


--------------------------------------------------------------------------------
SECTION 10 — FINAL DESIGN CHECKLIST
--------------------------------------------------------------------------------

Before shipping any visual mode, verify these points:

  Eye Health
  - Maximum default brightness is below 65 percent
  - No brightness strobe between 10 Hz and 50 Hz
  - Maximum saturation for sustained display is below 80 percent
  - All transitions use non-linear easing curves
  - The mode passes the campfire test (comfortable after 2 hours)
  - The session comfort fade is implemented

  Audio Sync Quality
  - Attack time matches the audio transient character
  - Release time feels musical, not mechanical
  - Stereo channels are handled independently
  - Mid and side channels each have visual representation
  - Mode transitions are triggered by musical events, not timers

  Engagement
  - Three-layer animation system is present (breath, rhythm, texture)
  - Rare events are implemented in this mode
  - Anticipation state is designed for quiet audio sections
  - The mode has a discoverable "personality" that pairs with specific music
  - First 30 seconds makes a new user feel something

  Technical
  - FPS cap is appropriate for the mode intensity
  - Audio analysis rate matches render rate
  - Chromatic aberration and organic noise are applied
  - Edge falloff uses exponential or custom curve
  - Mono audio fallback behavior is graceful


================================================================================
                          END OF GUIDE
        Build something that makes developers feel music.
        Not just see it. Feel it.
================================================================================