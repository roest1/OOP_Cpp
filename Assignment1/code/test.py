import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

divisorSum_inputTimes = pd.DataFrame({
    'x': [10000, 20000, 40000, 80000],
    'y': [63.7198, 255.058, 1014.25, 4057.44]
})

fasterSum_inputTimes = pd.DataFrame({
    'x': [10000, 20000, 40000, 80000],
    'y': [1.2183, 3.18773, 8.46491, 22.7776]
})

x = np.linspace(0, 33_000_000, 1000)

dSum_slope, dSum_intercept = np.polyfit(divisorSum_inputTimes['x'], divisorSum_inputTimes['y'], 1)
fSum_slope, fSum_intercept = np.polyfit(fasterSum_inputTimes['x'], fasterSum_inputTimes['y'], 1)


divisorSum_predicted_ms = dSum_slope * x + dSum_intercept
fasterSum_predicted_ms = fSum_slope * x + fSum_intercept

divisorSum_predicted_seconds = divisorSum_predicted_ms / 1000
fasterSum_predicted_seconds = fasterSum_predicted_ms / 1000

print(f"{int(divisorSum_predicted_seconds[-1])} days to calculate the 5ᵗʰ perfect number using divisorSum")
print(f"{int(fasterSum_predicted_seconds[-1])} days to calculate the 5ᵗʰ perfect number using fast sum")


plt.plot(x, divisorSum_predicted_seconds, label='divisorSum')
plt.plot(x, fasterSum_predicted_seconds, label='fasterSum')

plt.annotate(f'{fasterSum_predicted_seconds[-1]:.2f} days', (33_000_000, fasterSum_predicted_seconds[-1]), textcoords="offset points", xytext=(-10,10), ha='center')
plt.annotate(f'{divisorSum_predicted_seconds[-1]:.2f} days', (33_000_000, divisorSum_predicted_seconds[-1]), textcoords="offset points", xytext=(-10,10), ha='center')

plt.title('Trend of Time Taken to Calculate Perfect Numbers')
plt.xlabel('Number of Perfect Numbers')
plt.ylabel('Time Taken (Days)')
plt.legend()
plt.show()

