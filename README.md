# 🚗 Virtual Diagnostic Server (VDS)

[![Build](https://img.shields.io/github/actions/workflow/status/Pils48/virtual_ecu/build-test.yml?label=build&logo=github)](https://github.com/Pils48/virtual_ecu/actions/workflows/build-test.yml)
[![Tests](https://img.shields.io/github/actions/workflow/status/Pils48/virtual_ecu/build-test.yml?label=tests&logo=github)](https://github.com/Pils48/virtual_ecu/actions/workflows/build-test.yml)

## 📋 Project Description

A **Virtual Diagnostic Server** implementing UDS (Unified Diagnostic Services) protocol over CAN interface. Designed for testing and developing automotive diagnostic systems without requiring physical ECU hardware.

## 🎯 Purpose

The VDS provides a complete simulation environment for:
- **Developers** testing diagnostic software
- **QA Engineers** validating diagnostic sequences  
- **Researchers** prototyping new diagnostic features
- **Educators** teaching automotive diagnostics

## ✅ Supported UDS Services

| Service | Name | Status | Description |
|---------|------|---------|-------------|
| 0x3E | Tester Present | ✅ Implemented | Session keep-alive heartbeat |
| 0x10 | Diagnostic Session Control | ✅ Implemented | Session management and timing |
| 0x22 | Read Data By Identifier | ✅ Implemented | Read data using DIDs |
| 0x23 | Read Memory By Address | ✅ Implemented | Direct memory read access |
| 0x2E | Write Data By Identifier | ✅ Implemented | Write data using DIDs |
| 0x3D | Write Memory By Address | ✅ Implemented | Direct memory write access |

## 🏗️ Architecture Overview

Virtual Diagnostic Server (VDS)

├── 🔌 Transport Layer (CAN ISO-TP)

├── ⚡Protocol Handler (UDS)

├── 💾 Memory Emulation

└── 📊 Session Management
