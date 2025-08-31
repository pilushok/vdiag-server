# 🚗 Virtual Diagnostic Server (VDS)

[![Build and Test](https://github.com/Pils48/virtual_ecu/actions/workflows/build&test.yml/badge.svg)](https://github.com/Pils48/virtual_ecu/actions/workflows/build&test.yml)

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

## UDS Handlers

Our service handlers are designed as fully modular plugins, providing maximum flexibility and future-proofing for evolving transport layer requirements.

## Upcoming Features
🔜 Diagnostic Trouble Code (DTC) Handling - Implement full DTC management including reading, clearing, and status monitoring capabilities (0x19)

🔜 Security Access Services - Develop comprehensive security access control with authentication and encryption mechanisms (0x27)

🔜 InputOutput Control Implementation - Add support for vehicle I/O control and manipulation services (0x2F)

🔜 Routine Services - Implement automated routine execution and control capabilities (0x31)

🔜 Interchangeable Transport Layer - Create abstract transport interface supporting multiple protocols (Ethernet/DoIP, K-Line, etc.)
