# Crypto Arbitrage Monitor

A high-performance C++ system for real-time cryptocurrency arbitrage detection between Binance and Kraken exchanges.

## Overview

This system continuously monitors cryptocurrency trading pairs across Binance and Kraken exchanges to identify and analyze potential arbitrage opportunities. It employs a multi-threaded architecture for efficient data processing and real-time analysis.

## Features

- Real-time price monitoring across Binance and Kraken
- Multi-threaded architecture for parallel processing
- Configurable trading parameters via JSON configuration
- Automated viability analysis considering trading fees and minimum trade sizes
- Thread-safe logging and status updates
- Support for multiple trading pairs
- Customizable profit thresholds

## Dependencies

- nlohmann/json - For JSON parsing
- CPR (C++ Requests) - For HTTP requests
- Standard C++17 or later

## Configuration

The system is configured via a `config.json` file that should be placed in the parent directory. Example configuration structure:

```json
{
    "exchanges": {
        "binance": {
            "api_url": "https://api.binance.com/api/v3/trades",
            "req_timeout": 5000
        },
        "kraken": {
            "api_url": "https://api.kraken.com/0/public/Trades",
            "req_timeout": 5000
        }
    },
    "symbols": [
        "BTC/USDT",
        "ETH/USDT"
    ],
    "num_threads": 4,
    "feedback_interval": 60,
    "min_profit_percentage": 0.5,
    "trading_fees": 0.1,
    "min_trade_size": 0.001
}
```

## System Architecture

### Key Components

1. **ThreadedArbitrageSystem**: Main system coordinator that manages threads and data flow
2. **ArbitrageAnalyzer**: Analyzes price differences and identifies opportunities
3. **Exchange Classes**: Handles API communication with specific exchanges
4. **Config**: Singleton configuration manager
5. **Safe Output Utilities**: Thread-safe console output handling

### Data Flow

1. Exchange data is fetched for configured trading pairs
2. Data is queued for processing by worker threads
3. Worker threads analyze price differences
4. Viable opportunities are identified and reported
5. Status updates are provided at configured intervals

## Building and Running

### Prerequisites

- C++20 compatible compiler
- libcurl
- CPR library
- nlohmann/json library
- Homebrew (for macOS dependencies)

### Build Instructions

The project uses a Makefile build system. The default configuration assumes you're using Homebrew on macOS for managing dependencies.

Directory structure:
```
.
├── bin/           # Compiled binary
├── include/       # Header files
├── obj/          # Object files
└── src/          # Source files
```

To build the project:

```bash
# Install dependencies (macOS)
brew install curl cpr nlohmann-json

# Build the project
make

# Clean build files
make clean

# Rebuild everything
make rebuild
```

The compiled binary will be created at `bin/cryptoArbitrageMonitor`

#### Makefile Configuration

The project uses the following compiler settings:
- C++20 standard
- Wall, Wextra, and Wpedantic warning flags
- Includes from `/opt/homebrew/include`
- Links against curl and cpr libraries

### Running the System

```bash
./crypto_arbitrage
```

## Error Handling

The system includes robust error handling for:
- API communication failures
- JSON parsing errors
- Configuration loading issues
- Thread management
- Network timeouts

## Status Updates

The system provides regular status updates including:
- Number of trading pairs processed
- Time elapsed
- Opportunities found
- Viable opportunities with profit percentages

## Contributing

Feel free to submit issues and pull requests for:
- Additional exchange support
- New features
- Bug fixes
- Performance improvements
- Documentation updates

## License

[Add your chosen license here]

## Disclaimer

This software is for educational purposes only. Cryptocurrency trading carries significant risks. Always perform your own research and risk assessment before engaging in any trading activities.
